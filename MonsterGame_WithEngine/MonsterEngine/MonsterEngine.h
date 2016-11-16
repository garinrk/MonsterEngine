#pragma once

#include <stdlib.h>
#include <time.h>

#include "MonsterAllocator.h"
#include "MonsterDebug.h"
#include "MMath.h"

namespace MonsterEngine {
	int RandomTrueOrFalse();
	inline int RandomIntInRange_Inclusive(int min, int max);
	


};

void * operator new(size_t n);

void operator delete(void * p);

void * operator new[](size_t n);

void operator delete[](void * p);

#include "MonsterEngine-inl.h"
