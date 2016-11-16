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
	MonsterAllocator* inst = MonsterAllocator::getInstance();
	void *ptr = inst->MonsterMalloc(n);

	return ptr;

}
void operator delete(void * p)
{
	MonsterAllocator* inst = MonsterAllocator::getInstance();
	
	assert(inst->MonsterFree(p));

}
void * operator new[](size_t n)
{
	void * ptr = NULL;
	return ptr;
}
void operator delete[](void * p)
{
	
}

