#pragma once

#include <stdlib.h>
#include <time.h>

#include "GAllocator.h"
#include "MonsterDebug.h"
#include "MemoryManager.h"
#include "MMath.h"

namespace MonsterEngine {
	int RandomTrueOrFalse();

};
void * operator new(size_t n);

void * operator new(size_t n, const char * msg);

void operator delete(void * p);

void operator delete(void * p, const char * msg);

void * operator new[](size_t n);

void operator delete[](void * p);

void operator delete[](void * p, const char * msg);

#include "MonsterEngine-inl.h"
