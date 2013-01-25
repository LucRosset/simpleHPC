#include <vector>

#include <repast_hpc/AgentId.h>

#include "provider.h"

void Provider::provideContent(Bird* agent, std::vector<BirdPackage>& out) {
    repast::AgentId id = agent->getId();
    BirdPackage package = {id.id(), id.startingRank(), id.agentType (), agent->getHeading(), agent->getSpeed()};
    out.push_back(package);
}