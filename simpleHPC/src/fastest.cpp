#include "fastest.h"

Fastest::Fastest(BirdModel* _birds) {
	birds = _birds;
}

Fastest::~Fastest() {
}

int Fastest::getData() {
	return birds->countFastest();
}
