#include "FixedSizeAllocator.h"


FixedSizeAllocator* FixedSizeAllocator::Create(GAllocator* my_allocator, size_t amt_of_blocks, size_t initial_size_of_blocks, GAllocator* allocator) {
	
	//TODO: Make sure that the initial size is a power of two

	size_t amount_of_bytes = initial_size_of_blocks * amt_of_blocks + sizeof(FixedSizeAllocator);

	void* memory_chunk = my_allocator->GAlloc(amount_of_bytes);

	size_t* blocks_base_address = reinterpret_cast<size_t*>(reinterpret_cast<uintptr_t>(memory_chunk) + sizeof(FixedSizeAllocator));
	
	//using placement new to create fsa, bypassing overridden new 
	return new (memory_chunk) FixedSizeAllocator(initial_size_of_blocks, amt_of_blocks, blocks_base_address, amount_of_bytes,allocator);
}

void * FixedSizeAllocator::Falloc(size_t amt)
{
	assert(amt <= size_of_blocks_); //make sure we don't ask for more than possible

	if (amt > size_of_blocks_)
		return nullptr;

	size_t free_block_check = -1;
	//return null if there are no blocks left
	if (!bit_array_->GetFirstClearBit(free_block_check)) {
		if (free_block_check == -1) {
			return nullptr;
		}
	}
	//get first block that is free
	size_t free_block = 0;
	bit_array_->GetFirstClearBit(free_block);

	uint8_t * addr_for_user = reinterpret_cast<uint8_t*>(base_address_) +(free_block * size_of_blocks_);

	bit_array_->SetBit(free_block);

	return static_cast<void*>(addr_for_user);
}

bool FixedSizeAllocator::Free(void * addr_to_check)
{
	//double free check when there's nothing to free
	assert(!bit_array_->AreAllClear());
	if (bit_array_->AreAllClear()) {
		return false;
	}

	//make sure valid address
	assert(ContainedInAllocator(addr_to_check));
	if (!ContainedInAllocator(addr_to_check)) {
		return false;
	}

	//which block should we free?
	size_t block_to_free = static_cast<size_t>(reinterpret_cast<uint8_t*>(addr_to_check) - reinterpret_cast<uint8_t*>(base_address_)) / size_of_blocks_;

	//double free check
	assert(bit_array_->IsSet(block_to_free));
	if (bit_array_->IsClear(block_to_free)) {
		return false;
	}

	//free the block
	bit_array_->ClearBit(block_to_free);

	//TODO: Maybe memset the memory returned?
	return true;
}

bool FixedSizeAllocator::ContainedInAllocator(const void * addr_to_check) const
{
	uint8_t* back_of_chunk = reinterpret_cast<uint8_t*>(base_address_) + total_size_of_FSA_;

	if (addr_to_check <= back_of_chunk && addr_to_check >= base_address_) {
		return true;
	}
	else {
		return false;
	}
}

FixedSizeAllocator::FixedSizeAllocator(const size_t size_of_blocks, const size_t number_of_blocks, size_t* base_of_blocks, size_t total_size,GAllocator* allocator) :
	size_of_blocks_(size_of_blocks),
	num_of_blocks_(number_of_blocks),
	base_address_(base_of_blocks),
	total_size_of_FSA_(total_size)
{

	//zero out the memory
	bit_array_ = BitArray::Create(number_of_blocks, allocator);
	memset(base_address_, 0x00, total_size_of_FSA_ - sizeof(FixedSizeAllocator));
}


FixedSizeAllocator::~FixedSizeAllocator()
{
	//check for outstanding allocations
	if (!bit_array_->AreAllClear()) {
		DEBUGLOG("OUTSTANDING ALLOCATIONS IN FIXED SIZE ALLOCATOR");
	}
}
