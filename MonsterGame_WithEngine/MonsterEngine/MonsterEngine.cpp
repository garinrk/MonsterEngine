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

//regular new using our monsterallocator
void * operator new(size_t n)
{
	return	MonsterAllocator::getInstance()->MonsterMalloc(n);
}

//support for debug logs
void * operator new(size_t n, const char * msg) {
	void * ptr = NULL;
	DEBUGLOG(msg);
	return ptr;
}

//delete using our monsterallocator
void operator delete(void * p)
{
	MonsterAllocator* inst = MonsterAllocator::getInstance();
	bool val = inst->MonsterFree(p);

	assert(val);

}

//delete using debug log, will never get called. All funnels down to regular 
//delete functions
void operator delete(void * p, const char * msg)
{
	MonsterAllocator* inst = MonsterAllocator::getInstance();
	bool val = inst->MonsterFree(p);
	DEBUGLOG(msg);
	assert(val);
}

//regular new[] using our monsterallocator
void * operator new[](size_t n)
{
	return	MonsterAllocator::getInstance()->MonsterMalloc(n);
}

//delete[] using our monster allocator
void operator delete[](void * p)
{
	MonsterAllocator* inst = MonsterAllocator::getInstance();
	bool val = inst->MonsterFree(p);

	assert(val);
}

//delete using debug log, won't get called.
void operator delete[](void * p, const char * msg) {
	MonsterAllocator* inst = MonsterAllocator::getInstance();
	bool val = inst->MonsterFree(p);

	assert(val);
	DEBUGLOG(msg);

}

