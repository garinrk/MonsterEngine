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
	return GAllocator::GetInstance()->GAlloc(n);
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
	GAllocator* inst = GAllocator::GetInstance();
	bool result = inst->GFree(p);
	assert(result);

}

//delete using debug log, will never get called. All funnels down to regular 
//delete functions
void operator delete(void * p, const char * msg)
{
	GAllocator* inst = GAllocator::GetInstance();
	bool result = inst->GFree(p);;
	DEBUGLOG(msg);
	assert(result);
}

//regular new[] using our monsterallocator
void * operator new[](size_t n)
{
	return	GAllocator::GetInstance()->GAlloc(n);
}

//delete[] using our monster allocator
void operator delete[](void * p)
{
	GAllocator* inst = GAllocator::GetInstance();
	bool val = inst->GFree(p);

	assert(val);
}

//delete using debug log, won't get called.
void operator delete[](void * p, const char * msg) {
	GAllocator* inst = GAllocator::GetInstance();
	bool val = inst->GFree(p);

	assert(val);
	DEBUGLOG(msg);

}

