#pragma once

#include <stdlib.h>
#include <time.h>

#include "BlockAllocator.h"
#include "MMath.h"
#include "MemoryManager.h"
#include "MonsterDebug.h"


namespace MMEngine {
	int RandomTrueOrFalse();
};
//void * operator new(size_t n);
//
//void * operator new(size_t n, const char * msg);
//
//void * operator new(size_t n, const uint8_t alignment);
//
//void operator delete(void * p);
//
//void operator delete(void * p, const char * msg);
//
//void * operator new[](size_t n);
//
//void * operator new[](size_t n, const uint8_t alignment);
//
//void operator delete[](void * p);
//
//void operator delete[](void * p, const char * msg);
