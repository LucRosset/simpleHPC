#ifndef RECEIVER_H_INCLUDED
#define RECEIVER_H_INCLUDED

#include "bird.h"

class Receiver {
    Bird* createAgent(BirdPackage& content);
    void createAgents(std::vector<BirdPackage>& contents, std::vector<Bird*>& out);
};

#endif // RECEIVER_H_INCLUDED