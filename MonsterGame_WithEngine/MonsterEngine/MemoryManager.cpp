#include "MemoryManager.h"

MemoryManager* MemoryManager::manager_instance_  = NULL;
void* MemoryManager::singleton_addr_ = NULL;

MemoryManager::MemoryManager(const size_t block_allocator_size, const unsigned int num_of_descriptors, const uint8_t initial_alignment) {
	//create block allocator
	GAllocator::CreateInstance(block_allocator_size, num_of_descriptors, initial_alignment);
	block_allocator_ = GAllocator::GetInstance();

	//create FSAs
	for (size_t index = 0; index < SIZEOFSIZES; index++) {
		fixed_allocators_[index] = FixedSizeAllocator::Create(block_allocator_, DEFAULT_NUM_OF_FSA_BLOCKS, default_fsa_sizes[index], block_allocator_);
		assert(fixed_allocators_[index] != NULL);
	}


}

MemoryManager::~MemoryManager() {
	

	for (size_t index = 0; index < SIZEOFSIZES; index++) {
		fixed_allocators_[index]->~FixedSizeAllocator();
	}

	block_allocator_->~GAllocator();
}

void * MemoryManager::Malloc(const size_t amt)
{

	
	void* user_ptr = nullptr;
	size_t fsa_to_use = -1;
	//look through FSAs that satisfy the requirement
	for (size_t i = 0; i < SIZEOFSIZES; i++) {

		//attempt to malloc, will return a valid ptr if there's 
		//availability and it satisfies the size requirement
		user_ptr = fixed_allocators_[i]->Falloc(amt);

		if (user_ptr) {
			break;
		}

	}

	//nothing available in the FSAs, so just use the regular block allocator
	if (!user_ptr) {
		user_ptr = block_allocator_->GAlloc(amt);
	}

	//nothing yet? Than use the block allocator
	return user_ptr;
}

void * MemoryManager::Malloc(const size_t amt, uint8_t alignment)
{
	//just use the block allocator for proper alignment requirement
	return block_allocator_->GAlloc(amt, alignment);
}

bool MemoryManager::Free(void * addr)
{

	bool free_status = false;
	//look through the FSA first
	for (size_t i = 0; i < SIZEOFSIZES; i++) {

		//attempt to malloc, will return a valid ptr if there's 
		//availability and it satisfies the size requirement
		free_status = fixed_allocators_[i]->Free(addr);

		if (free_status) {
			return true;
		}

	}

	//must not be in the fsa, so maybe it's in the block allocator?
	free_status = block_allocator_->GFree(addr);
	assert(free_status); //double fre check
	return free_status;
}

void MemoryManager::GarbageCollectBlockAllocator()
{
	//garbage collect the block allocator;
	block_allocator_->GGCollect();
}

bool MemoryManager::ContainsAddress(void * addr_to_check)
{
	for (size_t i = 0; i < SIZEOFSIZES; i++) {
		//if(fixed_allocators_[i]-)
	}
	return false;
}

MemoryManager * MemoryManager::GetInstance()
{
	if (!manager_instance_) {
		CreateInstance(DEFAULT_BLOCK_ALLOCATOR_SIZE, DEFAULT_NUM_OF_DESCRIPTORS, DEFAULT_ALIGNMENT);
	}

	return manager_instance_;
}

void MemoryManager::CreateInstance(const size_t block_allocator_size, const unsigned int num_of_descriptors, const int8_t initial_alignment)
{
	MemoryManager::singleton_addr_ = _aligned_malloc(sizeof(MemoryManager), 4);
	manager_instance_ = new (singleton_addr_) MemoryManager(block_allocator_size, num_of_descriptors, initial_alignment);
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
