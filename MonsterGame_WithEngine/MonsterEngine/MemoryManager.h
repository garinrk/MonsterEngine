#pragma once

#include "FixedSizeAllocator.h"
#include "GAllocator.h"
#include "MonsterDebug.h"

#include <inttypes.h>

#define DEFAULT_BLOCK_ALLOCATOR_SIZE 1024*1024*50
#define DEFAULT_NUM_OF_DESCRIPTORS 5192
#define DEFAULT_NUM_OF_FSA_BLOCKS 256
#define DEFAULT_BLOCK_ALLOCATOR_ALIGNMENT 4
#define MAX_FSA_ALLOCATION_SIZE 128

#define SIZEOFSIZES sizeof(default_fsa_sizes) / sizeof(size_t)

class MemoryManager
{
public:
	MemoryManager(const size_t i_blockAllocatorSize, const unsigned int i_amtOfDescriptors, const uint8_t i_initAlign);
	~MemoryManager();

	void* Malloc(const size_t i_amt);
	void* Malloc(const size_t i_amt, uint8_t i_align);

	bool Free (void* i_addr);
	
	void GarbageCollectBlockAllocator();
	bool is_init = false;

#pragma region Singleton Creation and Destruction
	static MemoryManager* GetInstance();
	static void CreateInstance(const size_t i_blockAllocatorSize, const unsigned int i_amtOfDescriptors, const int8_t i_initAlign);
	static void CreateInstance(); //default using defined parameters
	static void DestroyInstance();
#pragma endregion Singleton Creation and Destruction

	void PrintBlockAllocatorState();


private:
	GAllocator* block_allocator_;
	FixedSizeAllocator* fixed_allocators_[5];
	size_t default_fsa_sizes[5] = {8, 16, 32, 64, 128};

#pragma region Singleton Instance Management
	static MemoryManager* manager_instance_;
	static void* singleton_addr_;
#pragma endregion Singleton Instance Management
};

