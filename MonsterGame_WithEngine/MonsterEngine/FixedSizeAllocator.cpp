#include "FixedSizeAllocator.h"


FixedSizeAllocator* FixedSizeAllocator::Create(GAllocator* my_allocator, size_t amt_of_blocks, size_t initial_size_of_blocks) {
	
	//TODO: Make sure that the initial size is a power of two

	size_t amount_of_bytes = initial_size_of_blocks * amt_of_blocks + sizeof(FixedSizeAllocator);

	void* memory_chunk = my_allocator->GAlloc(amount_of_bytes);

	size_t* blocks_address = reinterpret_cast<size_t*>(reinterpret_cast<uintptr_t>(memory_chunk) + sizeof(FixedSizeAllocator));

	//create bit array to represent the blocks

	bit_array_ = BitArray::Create(amt_of_blocks, false, my_allocator);

	return new (memory_chunk) FixedSizeAllocator(initial_size_of_blocks, amt_of_blocks, blocks_address, amount_of_bytes);
}

void * FixedSizeAllocator::Falloc(size_t amt)
{
	assert(amt <= size_of_blocks_); //make sure we don't ask for more than possible

	//which block is free?
	return nullptr;
}

FixedSizeAllocator::FixedSizeAllocator(const size_t size_of_blocks, const size_t number_of_blocks, size_t* base_of_blocks, size_t total_size) :
	size_of_blocks_(size_of_blocks),
	num_of_blocks_(number_of_blocks),
	base_address_(base_of_blocks),
	total_size_of_FSA_(total_size)
{

	//zero out the memory

	memset(base_address_, 0x00, total_size_of_FSA_ - sizeof(FixedSizeAllocator));
}


FixedSizeAllocator::~FixedSizeAllocator()
{
}
