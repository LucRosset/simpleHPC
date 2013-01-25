#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include <repast_hpc/Properties.h>
#include <repast_hpc/RepastProcess.h>
#include <repast_hpc/Schedule.h>
#include <repast_hpc/SharedContext.h>
#include <repast_hpc/SharedSpace.h>

namespace mpi = boost::mpi;

class BirdModel {

private:
    repast::SharedContext<Bird> agents;
    repast::SharedGrids<Bird>::SharedStrictGrid* grid;
    repast::Properties props;
    int rank;

public:
    BirdModel(const std::string& propsFile, int argc, char* argv[], mpi::communicator* world);
    virtual ~BirdModel();
    void init();
    void initSchedule(repast::ScheduleRunner& runner);
    void step();
};

#endif // MODEL_H_INCLUDED