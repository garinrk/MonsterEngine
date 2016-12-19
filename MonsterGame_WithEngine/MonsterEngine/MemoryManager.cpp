#include "MemoryManager.h"

MemoryManager* MemoryManager::manager_instance_  = NULL;
void* MemoryManager::singleton_addr_ = NULL;

MemoryManager::MemoryManager(const size_t i_blockAllocatorSize, const unsigned int i_amtOfDescriptors, const uint8_t i_initAlign) {
	//create block allocator
	GAllocator::CreateInstance(i_blockAllocatorSize, i_amtOfDescriptors, i_initAlign);
	block_allocator_ = GAllocator::GetInstance();

	//create FSAs
	for (size_t index = 0; index < SIZEOFSIZES; index++) {
		fixed_allocators_[index] = FixedSizeAllocator::Create(block_allocator_, DEFAULT_NUM_OF_FSA_BLOCKS, default_fsa_sizes[index]);
		assert(fixed_allocators_[index] != NULL);
	}


}

MemoryManager::~MemoryManager() {
	
	//destroy FSAs
	for (size_t index = 0; index < SIZEOFSIZES; index++) {
		fixed_allocators_[index]->~FixedSizeAllocator();
		block_allocator_->GFree(fixed_allocators_[index]);
	}

	//destroy block allocator
	//block_allocator_->~GAllocator();
	GAllocator::DestroyInstance();
}

void * MemoryManager::Malloc(const size_t i_amt)
{

	//is this amount small enough to fit within our FSAs?
	void * user_ptr = NULL;
	if (i_amt <= MAX_FSA_ALLOCATION_SIZE) {
		//cool, see if there's room in our FSAs

		for (size_t i = 0; i < SIZEOFSIZES; i++) {

			//attempt to malloc, will return a valid ptr if there's 
			//availability and it satisfies the size requirement
			user_ptr = fixed_allocators_[i]->Falloc(i_amt);

			if (user_ptr) {
				return user_ptr;
			}

		}

		//if we didn't get anything from our FSAs, we need to go and grab it from our block allocator
		//so, just fall out and get it from the block allocator.


	}

	//just go get it from the block allocator
	user_ptr = Malloc(i_amt, 4);


	return user_ptr;

	//
	//void* user_ptr = nullptr;
	//size_t fsa_to_use = -1;
	////look through FSAs that satisfy the requirement
	//for (size_t i = 0; i < SIZEOFSIZES; i++) {

	//	//attempt to malloc, will return a valid ptr if there's 
	//	//availability and it satisfies the size requirement
	//	user_ptr = fixed_allocators_[i]->Falloc(i_amt);

	//	if (user_ptr) {
	//		break;
	//	}

	//}

	////nothing available in the FSAs, so just use the regular block allocator
	//if (!user_ptr) {
	//	user_ptr = block_allocator_->GAlloc(i_amt);
	//}

	////nothing yet? Than use the block allocator
	//return user_ptr;
}

void * MemoryManager::Malloc(const size_t i_amt, uint8_t i_align)
{
	//just use the block allocator for proper alignment requirement
	return block_allocator_->GAlloc(i_amt, i_align);
}

bool MemoryManager::Free(void * i_addr)
{

	//first, let's see if this address is within one of our FSAs

	for (size_t i = 0; i < SIZEOFSIZES; i++) {

		if (fixed_allocators_[i]->ContainedInAllocator(i_addr)) {
			return fixed_allocators_[i]->Free(i_addr);
		}
	}

	return block_allocator_->GFree(i_addr);

	//not in there? well, than it must be in the block allocator yo.

	//////look through the FSA first
	////for (size_t i = 0; i < SIZEOFSIZES; i++) {

	////	//attempt to malloc, will return a valid ptr if there's 
	////	//availability and it satisfies the size requirement

	////	if (fixed_allocators_[i]->ContainedInAllocator(i_addr)) {
	////		return fixed_allocators_[i]->Free(i_addr);
	////	}

	////}




	//////must not be in the fsa, so maybe it's in the block allocator?
	//////free_status = block_allocator_->GFree(i_addr);
	//////assert(free_status); //double fre check
	//////return free_status;
}

void MemoryManager::GarbageCollectBlockAllocator()
{
	//garbage collect the block allocator;
	block_allocator_->GGCollect();
}

MemoryManager * MemoryManager::GetInstance()
{
	if (!manager_instance_) {
		CreateInstance(DEFAULT_BLOCK_ALLOCATOR_SIZE, DEFAULT_NUM_OF_DESCRIPTORS, DEFAULT_ALIGNMENT);
	}

	return manager_instance_;
}

void MemoryManager::CreateInstance(const size_t i_blockAllocatorSize, const unsigned int i_amtOfDescriptors, const int8_t i_initAlign)
{
	MemoryManager::singleton_addr_ = _aligned_malloc(sizeof(MemoryManager), 4);
	manager_instance_ = new (singleton_addr_) MemoryManager(i_blockAllocatorSize, i_amtOfDescriptors, i_initAlign);
}

void MemoryManager::CreateInstance()
{
	MemoryManager::singleton_addr_ = _aligned_malloc(sizeof(MemoryManager), 4);
	manager_instance_ = new (singleton_addr_) MemoryManager(DEFAULT_BLOCK_ALLOCATOR_SIZE, DEFAULT_NUM_OF_DESCRIPTORS, DEFAULT_ALIGNMENT);
}

void MemoryManager::DestroyInstance()
{
	manager_instance_->~MemoryManager();

	_aligned_free(manager_instance_);
	manager_instance_ = NULL;
}

void MemoryManager::PrintBlockAllocatorState()
{
	block_allocator_->PrintAllocatorState();
}
