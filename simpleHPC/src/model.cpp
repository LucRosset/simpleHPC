#include <repast_hpc/AgentId.h>
#include <repast_hpc/Point.h>
#include <repast_hpc/Properties.h>
#include <repast_hpc/RepastProcess.h>
#include <repast_hpc/SharedContext.h>
#include <repast_hpc/SharedSpace.h>

#include "bird.h"
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

BirdModel::BirdModel(const std::string& propsFile, int argc, char* argv[], mpi::communicator* world) :
    props(propsFile, argc, argv, world) {

    repast::RepastProcess* rp = repast::RepastProcess::instance();
    rank = rp->rank();

    int x = repast::strToInt(props.getProperty(MAX_X)) - repast::strToInt(props.getProperty(MIN_X)) + 1;
    int y = repast::strToInt(props.getProperty(MAX_Y)) - repast::strToInt(props.getProperty(MIN_Y)) + 1;

    int procX = repast::strToInt(props.getProperty(PROC_X));
    int procY = repast::strToInt(props.getProperty(PROC_Y));

    std::vector<int> procDim;
    procDim.push_back(procX);
    procDim.push_back(procY);

    int gridBuffer = repast::strToInt(props.getProperty(GRID_BUFFER));

    // Create the Grid
    grid = new repast::SharedGrids<Bird>::SharedStrictGrid("grid ",
            repast::GridDimensions(repast::Point<int>(x, y)), procDim, gridBuffer, world);
    agents.addProjection(grid);

    int dimX = x / procX;
    int dimY = y / procY;

    int originX = grid->dimensions().origin().getX();
    int originY = grid->dimensions().origin().getY();

    // Create the agents
    Bird* bird;
    for(int i = 0; i < (dimX * dimY); i++) {
        repast::AgentId id(i, rank, 0);
        bird = new Bird(id, i * 2, i / 2);
        agents.addAgent(bird);
        grid->moveTo(bird, repast::Point<int>(originX + (i / dimX), originY + (i % dimY)));
        std::cout << id << " " << rank << " " << originX + (i / dimX) << " " << originY + (i % dimY) << std::endl;
    }
}

BirdModel::~BirdModel() {
}

void BirdModel::init() {
}

void BirdModel::initSchedule(repast::ScheduleRunner& runner) {
}

void BirdModel::step() {
}
