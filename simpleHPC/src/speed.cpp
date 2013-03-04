#include "speed.h"

Speed::Speed(BirdModel* _birds) {
	birds = _birds;
}

Speed::~Speed() {
}

int Speed::getData() {
	//
	// E AGORA...???
	//
	return birds->countFastest();
}
