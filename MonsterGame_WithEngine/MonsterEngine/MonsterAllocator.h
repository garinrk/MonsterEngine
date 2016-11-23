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


#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

#include "MonsterDebug.h"
#define GUARDBAND_VAL 0xFF
#define GUARDBAND_BYTES 4
#define ALIGNMENT 4
#define TOTAL_SIZE 1024*1024
#define NUM_DESCRIPTORS 256

class MonsterAllocator
{

public:
	MonsterAllocator(size_t size_of_chunk, const unsigned int num_of_descriptors, uint8_t initial_alignment);
	~MonsterAllocator();
	void * MonsterMalloc(size_t amt);
	void * MonsterMalloc(size_t amt, uint8_t align);
	bool MonsterFree(void * addr);
	void GarbageCollect();
	size_t total_bytes_;

	void PrintLists() const;
	void PrintAllocatedList() const;
	void PrintFreeList() const;
	void PrintUnallocatedList() const;
	bool Contains(const void * addr) const;
	bool isAllocated(const void * addr) const;

	size_t GetLargestFreeBlock() const;
	static void CreateInstance(size_t total_size_of_heap, const unsigned int num_of_descriptors, uint8_t align);
	static MonsterAllocator* getInstance();
	static void DestroyInstance();
	static bool IsPowerOfTwo(uint8_t input);

	//static MonsterAllocator* pub_instance();
private:
	void InitializeFreeList(const int num_of_descriptors);
	void AddToAllocated(BlockDescriptor* to_insert);
	void AddToUnallocated(BlockDescriptor* to_insert);
	void AddToFree(BlockDescriptor * to_add);
	bool GuardBandChecks(BlockDescriptor * to_check);
	

	void ConsolidateBlocks(BlockDescriptor* first, BlockDescriptor * second);

	BlockDescriptor * UnallocBlockSearch(const void * base_addr, BlockDescriptor * start) const;
	BlockDescriptor * FindSuitableUnallocBlock(size_t amt, uint8_t align) const;
	BlockDescriptor * RemoveFromList(const void * addr, BlockDescriptor * root);
	BlockDescriptor * StealFromBlock(BlockDescriptor * victim, size_t amt, uint8_t align);

	inline size_t GetAlignmentOffset(const void * addr,uint8_t align);
	
	void * front_of_chunk_;
	void * back_of_chunk_;

	BlockDescriptor * free_root_ = 0;
	BlockDescriptor * endf_of_free_ = 0;
	BlockDescriptor * allocated_root_ = 0;
	BlockDescriptor * unallocated_root_ = 0;
	BlockDescriptor * front_of_bd_;

	//static MonsterAllocator* priv_instance;

	
	static MonsterAllocator* single_;

	static void * singleton_addr_;


	


};
#include "MonsterAllocator-inl.h"



