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
	

	for (size_t index = 0; index < SIZEOFSIZES; index++) {
		fixed_allocators_[index]->~FixedSizeAllocator();
	}

	block_allocator_->~GAllocator();
}

void * MemoryManager::Malloc(const size_t i_amt)
{

	
	void* user_ptr = nullptr;
	size_t fsa_to_use = -1;
	//look through FSAs that satisfy the requirement
	for (size_t i = 0; i < SIZEOFSIZES; i++) {

		//attempt to malloc, will return a valid ptr if there's 
		//availability and it satisfies the size requirement
		user_ptr = fixed_allocators_[i]->Falloc(i_amt);

		if (user_ptr) {
			break;
		}

	}

	//nothing available in the FSAs, so just use the regular block allocator
	if (!user_ptr) {
		user_ptr = block_allocator_->GAlloc(i_amt);
	}

	//nothing yet? Than use the block allocator
	return user_ptr;
}

void * MemoryManager::Malloc(const size_t i_amt, uint8_t i_align)
{
	//just use the block allocator for proper alignment requirement
	return block_allocator_->GAlloc(i_amt, i_align);
}

bool MemoryManager::Free(void * i_addr)
{

	bool free_status = false;
	//look through the FSA first
	for (size_t i = 0; i < SIZEOFSIZES; i++) {

		//attempt to malloc, will return a valid ptr if there's 
		//availability and it satisfies the size requirement
		//free_status = fixed_allocators_[i]->Free(i_addr);

		if (fixed_allocators_[i]->ContainedInAllocator(i_addr)) {
			return fixed_allocators_[i]->Free(i_addr);
		}

	}

	return block_allocator_->GFree(i_addr);
	//must not be in the fsa, so maybe it's in the block allocator?
	//free_status = block_allocator_->GFree(i_addr);
	//assert(free_status); //double fre check
	//return free_status;
}

void MemoryManager::GarbageCollectBlockAllocator()
{
	//garbage collect the block allocator;
	block_allocator_->GGCollect();
}

bool MemoryManager::ContainsAddress(void * i_addr)
{
	//TODO: Do this
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
