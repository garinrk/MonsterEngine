#include "FixedSizeAllocator.h"


FixedSizeAllocator* FixedSizeAllocator::Create(GAllocator* i_allocator, size_t i_amtOfBlocks, size_t i_sizeOfBlock) {
	
	size_t amount_of_bytes = i_sizeOfBlock * i_amtOfBlocks + sizeof(FixedSizeAllocator);

	void* base_of_fsa = i_allocator->GAlloc(amount_of_bytes);

	size_t* blocks_base_address = reinterpret_cast<size_t*>(reinterpret_cast<uintptr_t>(base_of_fsa) + sizeof(FixedSizeAllocator));
	
	void* back_of_fsa = reinterpret_cast<void*>(static_cast<size_t*>(base_of_fsa) + i_sizeOfBlock * i_amtOfBlocks);

	//using placement new to create fsa, bypassing overridden new 
	return new (base_of_fsa) FixedSizeAllocator(i_sizeOfBlock, i_amtOfBlocks, blocks_base_address, amount_of_bytes, i_allocator, base_of_fsa, back_of_fsa);
}

void * FixedSizeAllocator::Falloc(size_t i_amt)
{

	if (i_amt > size_of_blocks_)
		return nullptr;

	size_t free_block_check = -1;

	//ran out of blocks
	if (bit_array_->AreAllSet()) {
		DEBUGLOG("RAN OUT OF BLOCKS IN %ud FSA", i_amt);
		return nullptr;
	}

	//get first block that is free
	size_t free_block = 0;

	bit_array_->GetFirstClearBit(free_block);

	uint8_t * addr_for_user = reinterpret_cast<uint8_t*>(base_address_) +(free_block * size_of_blocks_);

	bit_array_->SetBit(free_block);

	return static_cast<void*>(addr_for_user);
}

bool FixedSizeAllocator::Free(void * i_addrToCheck)
{
	//is it even in here? just in case...
	if (!ContainedInAllocator(i_addrToCheck)) {
		return false;
	}
	
	//cant free if there's nothing to free
	if (bit_array_->AreAllClear()) {
		return false;
	}

	//which block should we free?
	size_t block_to_free = static_cast<size_t>(reinterpret_cast<uint8_t*>(i_addrToCheck) - reinterpret_cast<uint8_t*>(base_address_)) / size_of_blocks_;

	//double free check
	assert(bit_array_->IsSet(block_to_free));
	if (bit_array_->IsClear(block_to_free)) {
		return false;
	}
	//free the block
	bit_array_->ClearBit(block_to_free);
	return true;
}

bool FixedSizeAllocator::ContainedInAllocator(const void * i_addrToCheck) const
{
	uint8_t* back_of_chunk = reinterpret_cast<uint8_t*>(base_address_) + total_size_of_FSA_;

	//AWW YEAH TERNARY OPERATORS
	return (i_addrToCheck <= back_of_chunk && i_addrToCheck >= base_address_) ? true : false;

}

FixedSizeAllocator::FixedSizeAllocator(const size_t i_initSizeOfBlocks, const size_t i_amtOfBlocks, size_t* i_baseOfBlocks, size_t i_totalSize, GAllocator* i_allocator, void* i_fsaBase, void* i_fsaBack) :
	size_of_blocks_(i_initSizeOfBlocks),
	num_of_blocks_(i_amtOfBlocks),
	base_address_(i_baseOfBlocks),
	total_size_of_FSA_(i_totalSize),
	front_of_fsa_(i_fsaBase),
	back_of_fsa_(i_fsaBack),
	block_allocator_(i_allocator)
{

	//zero out the memory and create bitarray
	bit_array_ = BitArray::Create(i_amtOfBlocks, i_allocator);
	memset(base_address_, 0x00, total_size_of_FSA_ - sizeof(FixedSizeAllocator));
}


FixedSizeAllocator::~FixedSizeAllocator()
{
	
	if (!bit_array_->AreAllClear()) {
		DEBUGLOG("OUTSTANDING ALLOCATIONS IN FIXED SIZE ALLOCATOR");
	}
	//check bit array and destroy it
	bit_array_->~BitArray();
	block_allocator_->GFree(bit_array_);

}

