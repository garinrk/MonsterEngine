#pragma once
#include "BitArray.h"
#include "GAllocator.h"

class FixedSizeAllocator
{
public:
	static FixedSizeAllocator* Create(GAllocator* i_allocator, size_t i_amt0fBlocks, size_t i_initialBlockSize);
	FixedSizeAllocator(const size_t i_initSizeOfBlocks, const size_t i_amtOfBlocks, size_t* i_baseOfBlocks, size_t i_totalSize, GAllocator* i_allocator, void* i_fsaBase, void* i_fsaBack);
	void* Falloc(size_t amt);
	bool Free(void* i_addrToCheck);

	~FixedSizeAllocator();
	BitArray* GetArray() { return bit_array_; };
private:
	BitArray* bit_array_;
	size_t size_of_blocks_;
	size_t num_of_blocks_;
	bool ContainedInAllocator(const void * i_addrToCheck) const;
	
	
	size_t* base_address_;

	void* front_of_fsa_;
	void* back_of_fsa_;

	size_t total_size_of_FSA_;
};

