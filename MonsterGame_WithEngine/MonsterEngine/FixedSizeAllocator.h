#pragma once
#include "BitArray.h"
#include "GAllocator.h"

class FixedSizeAllocator
{
public:
	FixedSizeAllocator* Create(GAllocator* my_allocator, size_t amt_of_blocks, size_t initial_size_of_blocks);
	
	void* Falloc(size_t amt);

	~FixedSizeAllocator();

private:
	BitArray* bit_array_;
	size_t size_of_blocks_;
	size_t num_of_blocks_;
	
	FixedSizeAllocator(const size_t size_of_blocks, const size_t number_of_blocks, size_t* base_of_blocks, size_t total_size);

	size_t* base_address_;
	size_t total_size_of_FSA_;
};

