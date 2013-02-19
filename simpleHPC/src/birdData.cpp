#include "birdData.h"

BirdData::BirdData(BirdModel* _birds) {
	birds = _birds;
}

BirdData::~BirdData() {
}

int BirdData::getData() {
	return birds->getTest();
}
