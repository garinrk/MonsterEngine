#include "MonsterEngine.h"

#define DEFAULT_BLOCK_ALLOCATOR_SIZE 1024*1024*50
#define DEFAULT_NUM_OF_DESCRIPTORS 5192
#define DEFAULT_NUM_OF_FSA_BLOCKS 64
#define DEFAULT_BLOCK_ALLOCATOR_ALIGNMENT 4

int MonsterEngine::RandomTrueOrFalse()
{
	srand((unsigned int)time(NULL));
	return rand() % 2;
}

//override operator new
void * operator new(size_t n)
{

	return GAllocator::GetInstance()->GAlloc(n);
}

//support for debug logs
void * operator new(size_t n, const char * msg) {
	DEBUGLOG(msg);

	return MemoryManager::GetInstance()->Malloc(n);

}

//override operator new with alignment
void * operator new(size_t n, uint8_t alignment) {
	return MemoryManager::GetInstance()->Malloc(n,alignment);
}


//override operator delete
void operator delete(void * p)
{
	bool result = GAllocator::GetInstance()->GFree(p);
	assert(result);
}

//delete using debug log, will never get called. All funnels down to regular 
//delete functions
void operator delete(void * p, const char * msg)
{
	DEBUGLOG(msg);
	bool result = GAllocator::GetInstance()->GFree(p);
	assert(result);
}

//override operator new[]
void * operator new[](size_t n)
{

	return GAllocator::GetInstance()->GAlloc(n);

}

//override operator new[] with alignment
void * operator new[](size_t n, uint8_t alignment)
{

	return GAllocator::GetInstance()->GAlloc(n, alignment);

}

//delete[] using our monster allocator
void operator delete[](void * p)
{
	bool result = GAllocator::GetInstance()->GFree(p);
	assert(result);
}

//delete using debug log, won't get called.
void operator delete[](void * p, const char * msg) {
	DEBUGLOG(msg);
	bool result = GAllocator::GetInstance()->GFree(p);
	assert(result);
	
}

