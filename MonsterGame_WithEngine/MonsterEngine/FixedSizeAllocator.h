#pragma once
#include "BitArray.h"
#include "BlockAllocator.h"

namespace MMEngine {
	class FixedSizeAllocator
	{
	public:

		//Creation and Deletion

		static FixedSizeAllocator* Create(BlockAllocator* i_allocator, size_t i_amt0fBlocks, size_t i_initialBlockSize);
		FixedSizeAllocator(const size_t i_initSizeOfBlocks, const size_t i_amtOfBlocks, size_t* i_baseOfBlocks, size_t i_totalSize, BlockAllocator* i_allocator, void* i_fsaBase, void* i_fsaBack);
		~FixedSizeAllocator();

		//utilities
		void* Falloc(size_t i_amt);
		bool Free(void* i_addrToCheck);

		//helper functions
		BitArray* GetArray() const { return bit_array_; };
		bool ContainedInAllocator(const void * i_addrToCheck) const;

	private:
		BitArray* bit_array_;
		size_t size_of_blocks_;
		size_t num_of_blocks_;
		size_t* base_address_;
		size_t total_size_of_FSA_;

		void* front_of_fsa_;
		void* back_of_fsa_;

		BlockAllocator* block_allocator_;
	};
}

