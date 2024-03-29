#include "MonsterEngine.h"

#define DEFAULT_BLOCK_ALLOCATOR_SIZE 1024*1024*50
#define DEFAULT_NUM_OF_DESCRIPTORS 5192
#define DEFAULT_NUM_OF_FSA_BLOCKS 256
#define DEFAULT_BLOCK_ALLOCATOR_ALIGNMENT 4

using namespace MMEngine;

int MMEngine::RandomTrueOrFalse()
{
	srand((unsigned int)time(NULL));
	return rand() % 2;
}
//
////regular new using our allocator
//void * operator new(size_t n)
//{
//
//	return MemoryManager::GetInstance()->Malloc(n);
//}
//
////support for debug logs
//void * operator new(size_t n, const char * msg) {
//	DEBUGLOG(msg);
//
//	return MemoryManager::GetInstance()->Malloc(n);
//
//}
//
//void * operator new(size_t n, const uint8_t alignment)
//{
//	return MemoryManager::GetInstance()->Malloc(n, alignment);
//}
//
////delete using our allocator
//void operator delete(void * p)
//{
//
//	bool result = MemoryManager::GetInstance()->Free(p);
//	assert(result);
//}
//
////delete using debug log, will never get called. All funnels down to regular 
////delete functions
//void operator delete(void * p, const char * msg)
//{
//	DEBUGLOG(msg);
//	bool result = MemoryManager::GetInstance()->Free(p);
//	assert(result);
//}
//
////regular new[] using our allocator
//void * operator new[](size_t n)
//{
//
//	return MemoryManager::GetInstance()->Malloc(n);
//
//	return nullptr;
//}
//
//void * operator new[](size_t n, const uint8_t alignment)
//{
//	return MemoryManager::GetInstance()->Malloc(n, alignment);
//}
//
////delete[] using our allocator
//void operator delete[](void * p)
//{
//	bool result = MemoryManager::GetInstance()->Free(p);
//	assert(result);
//}
//
////delete using debug log, won't get called.
//void operator delete[](void * p, const char * msg) {
//	DEBUGLOG(msg);
//	bool result = MemoryManager::GetInstance()->Free(p);
//	assert(result);
//	
//}

