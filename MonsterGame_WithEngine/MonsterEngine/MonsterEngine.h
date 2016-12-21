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

void * operator new(size_t n, const char * msg);

void operator delete(void * p);

void operator delete(void * p, const char * msg);

void * operator new[](size_t n);

void operator delete[](void * p);

void operator delete[](void * p, const char * msg);


#include "MonsterEngine-inl.h"
