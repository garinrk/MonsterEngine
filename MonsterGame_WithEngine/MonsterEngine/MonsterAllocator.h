#pragma once
#include <inttypes.h>
struct BlockDescriptor {
	BlockDescriptor * prev;
	void * block_base;
	void * user_ptr;
	size_t whole_block_size;
	size_t user_size;
	size_t offset_padding;
#if _DEBUG
	int id;
#endif // DEBUG
		BlockDescriptor * next;
};


#include <malloc.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#define GUARDBAND_VAL 0xFF
#define GUARDBAND_BYTES 4
#define ALIGNMENT 4
class MonsterAllocator
{

public:
	MonsterAllocator(size_t size_of_chunk, const unsigned int num_of_descriptors);
	~MonsterAllocator();
	void * MonsterMalloc(size_t amt);
	bool MonsterFree(void * addr);
	void GarbageCollect();
	size_t total_bytes_;

	void PrintLists() const;
	void PrintAllocatedList() const;
	void PrintFreeList() const;
	void PrintUnallocatedList() const;
	bool Contains(void * addr) const;
	bool isAllocated(void * addr) const;

	size_t GetLargestFreeBlock() const;

private:
	void InitializeFreeList(int num_of_descriptors);
	void AddToAllocated(BlockDescriptor* to_insert);
	void AddToUnallocated(BlockDescriptor* to_insert);
	void AddToFree(BlockDescriptor * to_add);
	bool GuardBandChecks(BlockDescriptor * to_check);
	

	void ConsolidateBlocks(BlockDescriptor* first, BlockDescriptor * second);

	BlockDescriptor * UnallocBlockSearch(void * base_addr, BlockDescriptor * start) const;
	BlockDescriptor * FindSuitableUnallocBlock(size_t amt) const;
	BlockDescriptor * RemoveFromList(void * addr, BlockDescriptor * root);
	BlockDescriptor * StealFromBlock(BlockDescriptor * victim, size_t amt);

	inline size_t GetAlignmentOffset(void * addr);
	
	void * front_of_chunk_;
	void * back_of_chunk_;

	BlockDescriptor * free_root_ = 0;
	BlockDescriptor * endf_of_free_ = 0;
	BlockDescriptor * allocated_root_ = 0;
	BlockDescriptor * unallocated_root_ = 0;
	BlockDescriptor * front_of_bd_;


};

#include "MonsterAllocator-inl.h"





