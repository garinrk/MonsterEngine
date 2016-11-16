#pragma once

#include <stdlib.h>
#include <time.h>

#include "MMath.h"
namespace MonsterEngine {
	int RandomTrueOrFalse();
	inline int RandomIntInRange_Inclusive(int min, int max);
};

#include "MonsterEngine-inl.h"
