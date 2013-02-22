#ifndef FASTEST_H_INCLUDED
#define FASTEST_H_INCLUDED

#include <repast_hpc/TDataSource.h>

class BirdModel;

class Fastest: public repast::TDataSource<int> {

private:
	BirdModel* birds;

public:
	Fastest(BirdModel* _birds);
	~Fastest();

	int getData();
};

#endif // FASTEST_H_INCLUDED
