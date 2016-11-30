#pragma once
struct _Descriptor {
	_Descriptor * prev;
	void * base;
	void * user_ptr;
	size_t master_size;
	size_t user_size;
	size_t offset_pad;

#if _DEBUG
	int debug_id;
#endif
	_Descriptor * next;
};

#include <assert.h>
#include <inttypes.h>
#include <malloc.h>

#include "MonsterDebug.h"

#define BAND_VAL 0xFF
#define BAND_SIZE 4
#define ALIGN 4

#define DEFAULT_SIZE 1024*1024
#define DEFAULT_DESCRIPTORS 256
class GAllocator
{
public:

	GAllocator(const size_t total_allocator_size, const unsigned int num_of_descriptors, const uint8_t alignment );

	void * GAlloc(const size_t amt);
	void * GAlloc(const size_t amt, const uint8_t alignment);

	bool GFree(const void* addr_to_free);
	void GGCollect();

	GAllocator* GetInstance();
	void DestroyInstance();

	~GAllocator();

	size_t total_bytes_left = 0;

	void PrintList(_Descriptor* root);

private:

	void InitializeFreeList(const unsigned int num_of_descriptors);
	void AddToAllocatedList(const _Descriptor* node_to_insert);
	void AddToUnallocatedList( _Descriptor* node_to_insert);
	void AddToFreeList(const _Descriptor* node_to_insert);

	bool CheckGuardBands(_Descriptor* node_to_check);

	bool CombineBlocks(_Descriptor* first, _Descriptor* second);

	_Descriptor * SearchForBlock(const void * addr_to_search_for, const _Descriptor* root_node) const;
	_Descriptor * FindSuitableUnallocatedBlock(const size_t amt, uint8_t alignemnt) const;
	_Descriptor * RemoveBlockFromList(const void* addr_to_search_for, const _Descriptor* root_node);
	
	size_t GetAlignmentOffset(const void* addr, uint8_t alignment);

#pragma region AllocatorMemberVars
	_Descriptor * free_root_ = 0;
	_Descriptor * tail_of_free_ = 0;
	_Descriptor * allocated_root_ = 0;
	_Descriptor * unallocated_root_ = 0;
	_Descriptor * front_of_pool_ = 0;

	void * front_of_chunk_;
	void * back_of_chunk;
#pragma endregion AllocatorMemberVars

#pragma region Singleton
	static GAllocator* singleton_instance_;
	static void * singleton_instance_addr_;


	void CreateInstance(const size_t total_allocator_size, const unsigned int num_of_descriptors, const uint8_t alignment);


#pragma endregion Singleton
};

