#ifndef BIRDDATA_H_INCLUDED
#define BIRDDATA_H_INCLUDED

#include <repast_hpc/TDataSource.h>

class BirdModel;

class BirdData: public repast::TDataSource<int> {

private:
	BirdModel* birds;

public:
	BirdData(BirdModel* _birds);
	~BirdData();

	int getData();
};

#endif // BIRDDATA_H_INCLUDED
