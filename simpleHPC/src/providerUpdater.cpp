#include "providerUpdater.h"

ProviderUpdater::ProviderUpdater(repast::SharedContext<Bird> _agents) {
	agents = _agents;
}

ProviderUpdater::~ProviderUpdater() {
}

Bird* ProviderUpdater::createAgent(BirdPackage& content) {
	return new Bird(content.getId(), content.heading, content.speed);
}

void ProviderUpdater::createAgents(std::vector<BirdPackage>& contents,
		std::vector<Bird*>& out) {
	for (std::vector<BirdPackage>::iterator agent = contents.begin();
			agent != contents.end(); ++agent) {
		out.push_back(new Bird(agent->getId(), agent->heading, agent->speed));
	}
}

void ProviderUpdater::provideContent(Bird* agent,
		std::vector<BirdPackage>& out) {
	repast::AgentId id = agent->getId();
	BirdPackage package = { id.id(), id.startingRank(), id.agentType(),
			agent->getHeading(), agent->getSpeed() };
	out.push_back(package);
}

void ProviderUpdater::provideContent(const repast::AgentRequest& request,
		std::vector<BirdPackage>& out) {
	const std::vector<repast::AgentId>& ids = request.requestedAgents();
	for (int i = 0, size = ids.size(); i < size; i++) {
		repast::AgentId id = ids[i];

		if (agents.contains(id)) {
			Bird* bird = agents.getAgent(id);
			BirdPackage content = { id.id(), id.startingRank(), id.agentType(),
					bird->getHeading(), bird->getSpeed() };
			out.push_back(content);
		}
	}
}

void ProviderUpdater::updateAgent(const BirdPackage& content) {
	repast::AgentId id = content.getId();

	if (agents.contains(id)) {
		Bird* copy = agents.getAgent(id);
		copy->setHeading(content.heading);
		copy->setSpeed(content.speed);
	}
}
