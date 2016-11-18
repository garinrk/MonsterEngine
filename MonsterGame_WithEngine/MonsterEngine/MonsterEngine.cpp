#include "MonsterEngine.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Random true or false. </summary>
///
/// <remarks>	garinrk, 10/23/2016. </remarks>
///
/// <returns>	An int, 1 = true. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

int MonsterEngine::RandomTrueOrFalse()
{
	srand((unsigned int)time(NULL));
	return rand() % 2;
}

void * operator new(size_t n)
{
	return	MonsterAllocator::getInstance()->MonsterMalloc(n);
}
void * operator new(size_t n, const char * msg) {
	void * ptr = NULL;
	DEBUGLOG(msg);
	return ptr;
}

void operator delete(void * p)
{
	MonsterAllocator* inst = MonsterAllocator::getInstance();
	bool val = inst->MonsterFree(p);

	assert(val);

}
void * operator new[](size_t n)
{
	return	MonsterAllocator::getInstance()->MonsterMalloc(n);
}
void operator delete[](void * p)
{
	MonsterAllocator* inst = MonsterAllocator::getInstance();
	bool val = inst->MonsterFree(p);

	assert(val);
}

void operator delete[](void * p, const char * msg) {
	MonsterAllocator* inst = MonsterAllocator::getInstance();
	bool val = inst->MonsterFree(p);

	assert(val);
	DEBUGLOG(msg);

}

