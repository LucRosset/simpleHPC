#ifndef BIRD_H_INCLUDED
#define BIRD_H_INCLUDED

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <repast_hpc/AgentId.h>

struct BirdPackage {

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & id;
        ar & proc;
        ar & type;
        ar & heading;
        ar & speed;
        ar & fastest;
    }

    int id;
    int proc;
    int type;
    float heading;
    float speed;
    bool fastest;

    repast::AgentId getId() const {
        return repast::AgentId(id, proc, type);
    }
};

class Bird : public repast::Agent {

    friend class boost::serialization::access;

private:
    repast::AgentId id;
    float heading;
    float speed;
    bool fastest;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & boost::serialization::base_object<Agent> ( *this );
        ar & id;
        ar & heading;
        ar & speed;
        ar & fastest;
    }

public:
    Bird(repast::AgentId, float heading, float speed);
    virtual ~Bird();

    float getHeading();
    void setHeading(float _heading);

    float getSpeed();
    void setSpeed(float _speed);

    bool getFastest();
    void setFastest(bool _fastest);

    void fly(int time);

    virtual repast::AgentId& getId() {
        return id;
    }

    virtual const repast::AgentId& getId() const {
        return id;
    }
};

#endif // BIRD_H_INCLUDED
