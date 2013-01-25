#ifndef PROVIDERUPDATER_H_INCLUDED
#define PROVIDERUPDATER_H_INCLUDED

#include <vector>

#include <repast_hpc/AgentRequest.h>
#include <repast_hpc/SharedContext.h>

#include "bird.h"

class ProviderUpdater {

private:
	repast::SharedContext<Bird> agents;

public:
	ProviderUpdater(repast::SharedContext<Bird> _agents);

	~ProviderUpdater();

	Bird* createAgent(BirdPackage& content);

	void createAgents(std::vector<BirdPackage>& contents,
			std::vector<Bird*>& out);

	void provideContent(Bird* agent, std::vector<BirdPackage>& out);

	void provideContent(const repast::AgentRequest& request,
			std::vector<BirdPackage>& out);

	void updateAgent(const BirdPackage& content);
};

#endif // PROVIDERUPDATER_H_INCLUDED
