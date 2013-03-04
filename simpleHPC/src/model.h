#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include <repast_hpc/AgentId.h>
#include <repast_hpc/AgentRequest.h>
#include <repast_hpc/Point.h>
#include <repast_hpc/Properties.h>
#include <repast_hpc/RepastProcess.h>
#include <repast_hpc/Schedule.h>
#include <repast_hpc/SharedContext.h>
#include <repast_hpc/SharedSpace.h>
#include <repast_hpc/SVDataSetBuilder.h>
#include <repast_hpc/Utilities.h>

#include "bird.h"
#include "birdData.h"
#include "fastest.h"
#include "providerUpdater.h"
#include "speed.h"

namespace mpi = boost::mpi;

class BirdModel {

private:
	int rank;
	int stopAt;
	int sizeX;
	int sizeY;
	int dimX;
	int dimY;
	repast::SharedContext<Bird> agents;
	repast::SharedGrids<Bird>::SharedWrappedGrid* grid;
	repast::Properties props;
	ProviderUpdater providerUpdater;

public:
	BirdModel(const std::string& propsFile, int argc, char* argv[],
			mpi::communicator* world);

	virtual ~BirdModel();

	void provideContent(const repast::AgentRequest& request,
			std::vector<BirdPackage>& out);

	void init();

	void initSchedule(repast::ScheduleRunner& runner);

	void step();

	void synchAgents();

	/**
	 * ONLY FOR TEST
	 */
	int getTest();

	int countFastest ();
};

#endif // MODEL_H_INCLUDED
