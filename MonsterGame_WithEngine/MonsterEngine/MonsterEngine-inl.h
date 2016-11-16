#include "MonsterEngine.h"
#pragma once

inline int MonsterEngine::RandomIntInRange_Inclusive(int min, int max) {
	return min + (rand() % (int)(max - min + 1));
}


