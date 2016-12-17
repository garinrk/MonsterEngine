#pragma once


#include "FixedSizeAllocator.h"
#include "GAllocator.h"
#include "MonsterDebug.h"

#include <inttypes.h>

#define DEFAULT_BLOCK_ALLOCATOR_SIZE 1024*1024*50
#define DEFAULT_NUM_OF_DESCRIPTORS 5192
#define DEFAULT_NUM_OF_FSA_BLOCKS 64
#define DEFAULT_BLOCK_ALLOCATOR_ALIGNMENT 4

#define SIZEOFSIZES sizeof(default_fsa_sizes) / sizeof(size_t)

class MemoryManager
{
public:
	MemoryManager(const size_t block_allocator_size, const unsigned int num_of_descriptors, const uint8_t initial_alignment);
	~MemoryManager();

	void* Malloc(const size_t amt);
	void* Malloc(const size_t amt, uint8_t alignment);

	bool Free (void* addr);
	
	void GarbageCollectBlockAllocator();
	bool is_init = false;

	bool ContainsAddress(void* addr_to_check);

	static MemoryManager* GetInstance();
	static void CreateInstance(const size_t block_allocator_size, const unsigned int num_of_descriptors, const int8_t initial_alignment);
	static void CreateInstance(); //default using defined parameters
	static void DestroyInstance();

private:
	GAllocator* block_allocator_;
	FixedSizeAllocator* fixed_allocators_[5];
	size_t default_fsa_sizes[5] = {8, 16, 32, 64, 128};

	static MemoryManager* manager_instance_;
	static void* singleton_addr_;

};

