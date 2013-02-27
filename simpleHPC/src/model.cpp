#include "model.h"

const std::string STOP_AT = "stop.at";
const std::string MIN_X = "min.x";
const std::string MIN_Y = "min.y";
const std::string MAX_X = "max.x";
const std::string MAX_Y = "max.y";
const std::string GRID_BUFFER = "grid.buffer";
const std::string PROC_X = "proc.per.x";
const std::string PROC_Y = "proc.per.y";

namespace mpi = boost::mpi;

BirdModel::BirdModel(const std::string& propsFile, int argc, char* argv[],
		mpi::communicator* world) :
		props(propsFile, argc, argv, world), providerUpdater(agents) {

	repast::RepastProcess* rp = repast::RepastProcess::instance();
	rank = rp->rank();

	stopAt = repast::strToInt(props.getProperty(STOP_AT));

	// Size of the grid
	sizeX = repast::strToInt(props.getProperty(MAX_X))
			- repast::strToInt(props.getProperty(MIN_X)) + 1;
	sizeY = repast::strToInt(props.getProperty(MAX_Y))
			- repast::strToInt(props.getProperty(MIN_Y)) + 1;

	int procX = repast::strToInt(props.getProperty(PROC_X));
	int procY = repast::strToInt(props.getProperty(PROC_Y));

	std::vector<int> procDim;
	procDim.push_back(procX);
	procDim.push_back(procY);

	int gridBuffer = repast::strToInt(props.getProperty(GRID_BUFFER));

	// Create the Grid
	grid = new repast::SharedGrids<Bird>::SharedWrappedGrid("grid ",
			repast::GridDimensions(repast::Point<int>(sizeX, sizeY)), procDim,
			gridBuffer, world);
	agents.addProjection(grid);

	// Size of the grid for managed by each process
	dimX = sizeX / procX;
	dimY = sizeY / procY;

	int originX = grid->dimensions().origin().getX();
	int originY = grid->dimensions().origin().getY();

	// Create the agents
	Bird* bird;
	for (int i = 0; i < (dimX * dimY); i++) {
		repast::AgentId id(i, rank, 0);
		bird = new Bird(id, i * 2, i / 2);
		agents.addAgent(bird);
		grid->moveTo(bird,
				repast::Point<int>(originX + (i / dimX), originY + (i % dimY)));
		std::cout << id << " " << rank << " " << originX + (i / dimX) << " "
				<< originY + (i % dimY) << std::endl;
	}

	grid->initSynchBuffer(agents);
	grid->synchBuffer<BirdPackage>(agents, providerUpdater, providerUpdater);
}

BirdModel::~BirdModel() {
}

void BirdModel::init() {
}

void BirdModel::initSchedule(repast::ScheduleRunner& runner) {
	runner.scheduleStop(stopAt);

	runner.scheduleEvent(1, 1,
			repast::Schedule::FunctorPtr(
					new repast::MethodFunctor<BirdModel>(this,
							&BirdModel::step)));

	runner.scheduleEvent(1.05, 1,
			repast::Schedule::FunctorPtr(
					new repast::MethodFunctor<BirdModel>(this,
							&BirdModel::synchAgents)));

	// Create a builder for netcdf aggregate data collection
	repast::SVDataSetBuilder testBuilder("./logs/data.csv", ";",
			repast::RepastProcess::instance()->getScheduleRunner().schedule());

	// This is the data source
	BirdData* bData = new BirdData(this);
	testBuilder.addDataSource(
			repast::createSVDataSource("RANDOM_NUMBERS", bData,
					std::plus<int>()));
	repast::DataSet* testDataSet = testBuilder.createDataSet();

	// Schedule the record and write on the dataset
	runner.scheduleEvent(1.1, 1,
			repast::Schedule::FunctorPtr(
					new repast::MethodFunctor<repast::DataSet>(testDataSet,
							&repast::DataSet::record)));

	repast::Schedule::FunctorPtr dataWrite = repast::Schedule::FunctorPtr(
			new repast::MethodFunctor<repast::DataSet>(testDataSet,
					&repast::DataSet::write));

	runner.scheduleEvent(1.2, 1, dataWrite);

	// Make sure we write the data when the sim ends
	runner.scheduleEndEvent(dataWrite);

	/**
	 * NETCDF TEST 2
	 */
	// Create a builder for netcdf aggregate data collection
	repast::SVDataSetBuilder fastBuilder("./logs/fastest.csv", ";",
			repast::RepastProcess::instance()->getScheduleRunner().schedule());

	// This is the data source
	Fastest* fast = new Fastest(this);
	fastBuilder.addDataSource(
			repast::createSVDataSource("NUM_OF_FASTEST_BIRDS", fast,
					std::plus<int>()));
	repast::DataSet* fastDataSet = fastBuilder.createDataSet();

	// Schedule the record and write on the dataset
	runner.scheduleEvent(1.1, 1,
			repast::Schedule::FunctorPtr(
					new repast::MethodFunctor<repast::DataSet>(fastDataSet,
							&repast::DataSet::record)));

	repast::Schedule::FunctorPtr fastWrite = repast::Schedule::FunctorPtr(
			new repast::MethodFunctor<repast::DataSet>(fastDataSet,
					&repast::DataSet::write));

	runner.scheduleEvent(1.2, 1, fastWrite);

	// Make sure we write the data when the sim ends
	runner.scheduleEndEvent(fastWrite);
}

void BirdModel::step() {
	//std::cout << "  " << rank << "  " << std::endl;

	std::vector<int> position(2);
	for (int i = 0; i < dimX * dimY; i++) {
		repast::AgentId id = repast::AgentId(i, rank, 0);
		if (grid->getLocation(id, position)) {
			Bird* thisBird = agents.getAgent(id);
			Bird* neighbour;
			int isFastest = 0;

			neighbour = grid->getObjectAt(
					repast::Point<int>(position[0], (position[1] + 1) % sizeY));
			isFastest += (neighbour->getSpeed() < thisBird->getSpeed());
			neighbour = grid->getObjectAt(
					repast::Point<int>((position[0] + 1) % sizeX, position[1]));
			isFastest += (neighbour->getSpeed() < thisBird->getSpeed());
			neighbour = grid->getObjectAt(
					repast::Point<int>(position[0], (position[1] - 1 + sizeY) % sizeY));
			isFastest += (neighbour->getSpeed() < thisBird->getSpeed());
			neighbour = grid->getObjectAt(
					repast::Point<int>((position[0] - 1 + sizeX) % sizeX, position[1]));
			isFastest += (neighbour->getSpeed() < thisBird->getSpeed());

			thisBird->setFastest((bool) isFastest);

			/*
			 Bird* bird_0 = agents.getAgent(id);
			 Bird* bird_1;
			 if (position[1] != sizeY - 1) // Check if agent is not in the lowest row
			 bird_1 = grid->getObjectAt(
			 repast::Point<int>(position[0], position[1] + 1));
			 else if (position[0] != sizeX - 1) // Check if agent is in the last column
			 bird_1 = grid->getObjectAt(
			 repast::Point<int>(position[0] + 1, 0));
			 else
			 // agent is in (sizeX-1 ; sizeY-1)
			 bird_1 = grid->getObjectAt(repast::Point<int>(0, 0));
			 std::cout << "THIS: " << bird_0->getId() << "   NEXT: "
			 << bird_1->getId() << std::endl;
			 */
		}
	}
}

void BirdModel::synchAgents() {
	repast::syncAgents<BirdPackage>(providerUpdater, providerUpdater);
	grid->synchBuffer<BirdPackage>(agents, providerUpdater, providerUpdater);
}

int BirdModel::getTest() {
	return std::rand() % 10;
}

int BirdModel::countFastest() {
	int num = 0;
	Bird* bird;
	for (int i = 0; i < dimX * dimY; i++) {
		bird = agents.getAgent(repast::AgentId(i, rank, 0));
		if (bird->getFastest())
			num += 1;
	}
	return num;
}
