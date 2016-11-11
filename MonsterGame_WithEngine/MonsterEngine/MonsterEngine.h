#ifndef MONSTERENGINE_H
#define MONSTERENGINE_H
#include "MMath.h"
#include <stdlib.h>
#include <time.h>
namespace MonsterEngine {
	int RandomTrueOrFalse();
	inline int RandomIntInRange_Inclusive(int min, int max);
};

#include "MonsterEngine-inl.h"
#endif
