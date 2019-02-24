#include "StageClass.h"
#include "Actor.h"
#include "BackGroundUIClass.h"

StageClass::StageClass(std::vector<Actor*>& Stages) {
	Stages.push_back(this);
}

StageClass::~StageClass() {
}