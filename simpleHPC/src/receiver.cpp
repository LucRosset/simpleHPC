#include <vector>

#include "receiver.h"

Bird* Receiver::createAgent(BirdPackage& content){
    return new Bird(content.getId(), content.heading, content.speed);
}

void Receiver::createAgents(std::vector<BirdPackage>& contents, std::vector<Bird*>& out) {
    for (std::vector<BirdPackage>::iterator agent = contents.begin(); agent != contents.end(); ++agent){
        out.push_back(new Bird(agent->getId(), agent->heading, agent->speed));
    }
}