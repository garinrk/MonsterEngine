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
class MonsterAllocator
{

public:
	MonsterAllocator(size_t size_of_chunk, const unsigned int num_of_descriptors);
	~MonsterAllocator();
	void * MonsterMalloc(size_t amt);
	bool MonsterFree(void * addr);
	void GarbageCollect();
	size_t total_bytes_;

	void PrintLists();
	void PrintAllocatedList();
	void PrintFreeList();
	void PrintUnallocatedList();
	bool Contains(void * addr);
	bool isAllocated(void * addr);

	size_t GetLargestFreeBlock();

private:
	void InitializeFreeList(int num_of_descriptors);
	void AddToAllocated(BlockDescriptor* to_insert);
	void AddToUnallocated(BlockDescriptor* to_insert);
	void AddToFree(BlockDescriptor * to_add);
	bool GuardBandChecks(BlockDescriptor * to_check);
	

	void ConsolidateBlocks(BlockDescriptor* first, BlockDescriptor* second);

	BlockDescriptor * UnallocBlockSearch(void * base_addr, BlockDescriptor * start);
	BlockDescriptor * FindSuitableUnallocBlock(size_t amt);
	BlockDescriptor * RemoveFromList(void * addr, BlockDescriptor * root);
	BlockDescriptor * StealFromBlock(BlockDescriptor * victim, size_t amt);

	size_t GetAlignmentOffset(void * addr);
	
	void * front_of_chunk_;
	void * back_of_chunk_;

	BlockDescriptor * free_root_ = 0;
	BlockDescriptor * endf_of_free_ = 0;
	BlockDescriptor * allocated_root_ = 0;
	BlockDescriptor * unallocated_root_ = 0;
	BlockDescriptor * front_of_bd_;


};





