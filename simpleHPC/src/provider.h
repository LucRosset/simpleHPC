#ifndef PROVIDER_H_INCLUDED
#define PROVIDER_H_INCLUDED

#include "bird.h"

class Provider {
    void provideContent(Bird* agent, std::vector<BirdPackage>& out);
};

#endif // PROVIDER_H_INCLUDED