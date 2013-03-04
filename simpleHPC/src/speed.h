#ifndef SPEED_H_INCLUDED
#define SPEED_H_INCLUDED

#include <repast_hpc/TDataSource.h>

#include "model.h"

class BirdModel;

class Speed: public repast::TDataSource<int> {

private:
	BirdModel* birds;

public:
	Speed(BirdModel* _birds);
	~Speed();

	int getData();
};

#endif // SPEED_H_INCLUDED
