#include "MonsterEngine.h"

#define DEFAULT_BLOCK_ALLOCATOR_SIZE 1024*1024*50
#define DEFAULT_NUM_OF_DESCRIPTORS 5192
#define DEFAULT_NUM_OF_FSA_BLOCKS 64
#define DEFAULT_BLOCK_ALLOCATOR_ALIGNMENT 4

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
//
////regular new using our monsterallocator
//void * operator new(size_t n)
//{
//	if (!MemoryManager::is_init) {
//		//create memory manager
//		
//		void * managers_address = _aligned_malloc(sizeof(MemoryManager) + sizeof(GAllocator),4);
//		
//		MemoryManager* m_mgr = new (managers_address) MemoryManager(DEFAULT_BLOCK_ALLOCATOR_SIZE, DEFAULT_NUM_OF_DESCRIPTORS, DEFAULT_BLOCK_ALLOCATOR_ALIGNMENT);
//		MemoryManager::is_init = true;
//		MemoryManager::SetInstance(m_mgr);
//		return m_mgr->Malloc(n);
//	}
//	else {
//		return MemoryManager::GetInstance()->Malloc(n);
//	}
//}
//
////support for debug logs
//void * operator new(size_t n, const char * msg) {
//	DEBUGLOG(msg);
//	
//	if (!MemoryManager::is_init) {
//		//create memory manager
//
//		void * managers_address = _aligned_malloc(sizeof(MemoryManager) + sizeof(GAllocator), 4);
//
//		MemoryManager* m_mgr = new (managers_address) MemoryManager(DEFAULT_BLOCK_ALLOCATOR_SIZE, DEFAULT_NUM_OF_DESCRIPTORS, DEFAULT_BLOCK_ALLOCATOR_ALIGNMENT);
//		MemoryManager::is_init = true;
//		MemoryManager::SetInstance(m_mgr);
//		return m_mgr->Malloc(n);
//	}
//	else {
//		return MemoryManager::GetInstance()->Malloc(n);
//	}
//}
//
////delete using our monsterallocator
//void operator delete(void * p)
//{
//	if (!MemoryManager::is_init) {
//		return;
//	}
//	bool result = MemoryManager::GetInstance()->Free(p);
//	assert(result);
//
//}
//
////delete using debug log, will never get called. All funnels down to regular 
////delete functions
//void operator delete(void * p, const char * msg)
//{
//	DEBUGLOG(msg);
//	if (!MemoryManager::is_init) {
//		return;
//	}
//	bool result = MemoryManager::GetInstance()->Free(p);
//	assert(result);
//}
//
////regular new[] using our monsterallocator
//void * operator new[](size_t n)
//{
//	//return	GAllocator::GetInstance()->GAlloc(n);
//	if (!MemoryManager::is_init) {
//		//create memory manager
//
//		void * managers_address = _aligned_malloc(sizeof(MemoryManager) + sizeof(GAllocator), 4);
//
//		MemoryManager* m_mgr = new (managers_address) MemoryManager(DEFAULT_BLOCK_ALLOCATOR_SIZE, DEFAULT_NUM_OF_DESCRIPTORS, DEFAULT_BLOCK_ALLOCATOR_ALIGNMENT);
//		MemoryManager::is_init = true;
//		MemoryManager::SetInstance(m_mgr);
//
//		return m_mgr->Malloc(n);
//	}
//	else {
//		return MemoryManager::GetInstance()->Malloc(n);
//	}
//}
//
////delete[] using our monster allocator
//void operator delete[](void * p)
//{
//	/*GAllocator* inst = GAllocator::GetInstance();
//	bool val = inst->GFree(p);*/
//	if (!MemoryManager::is_init) {
//		return;
//	}
//	bool result = MemoryManager::GetInstance()->Free(p);
//	assert(result);
//}
//
////delete using debug log, won't get called.
//void operator delete[](void * p, const char * msg) {
//	if (!MemoryManager::is_init) {
//		return;
//	}
//	bool result = MemoryManager::GetInstance()->Free(p);
//	assert(result);
//	DEBUGLOG(msg);
//
//}
//
