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
#include <repast_hpc/Utilities.h>

#include "providerUpdater.h"

namespace mpi = boost::mpi;

class BirdModel {

private:
	int rank;
	int stopAt;
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
};

#endif // MODEL_H_INCLUDED
