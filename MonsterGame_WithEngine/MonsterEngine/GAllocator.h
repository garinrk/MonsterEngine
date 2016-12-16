#pragma once
struct _Descriptor {
	_Descriptor * prev;
	void * base;
	void * user_ptr;
	size_t master_size;
	size_t user_size;
	//size_t offset_pad;

#if _DEBUG
	int debug_id;
#endif
	_Descriptor * next;
};


#include <inttypes.h>
#include "MonsterDebug.h"

#define BAND_VAL 0xFD
#define BAND_SIZE 4


#define DEFAULT_ALIGNMENT 4
#define DEFAULT_TOTAL_SIZE 1024*1024*100 // 100mb, to handle std::vector and up to 128 byte wide FSAs
#define DEFAULT_NUM_DESCRIPTORS 5192*2
class GAllocator
{
public:

	GAllocator(const size_t total_allocator_size, const unsigned int num_of_descriptors, const uint8_t alignment );

	void * GAlloc(const size_t amt);
	void * GAlloc(const size_t amt, const uint8_t alignment);

	bool GFree(const void* addr_to_free);
	void GGCollect();

	static void CreateInstance(const size_t total_allocator_size, const unsigned int num_of_descriptors, const uint8_t alignment);
	static void CreateInstance(); //default using defined sizes
	static GAllocator* GetInstance();
	static void DestroyInstance();

	~GAllocator();

	size_t total_bytes_left = 0;

	void PrintList(_Descriptor* root);

	//For unit tests
	bool ContainsAddressInBlock(const void* addr_to_find);
	bool IsAllocatedAddress(const void* addr_to_find);
	size_t GetLargestFreeBlockSize();

	//debug
	void PrintAllocatorState();
private:

	void InitializeFreeList(const unsigned int num_of_descriptors);
	void AddToAllocatedList(_Descriptor* node_to_insert);
	void AddToUnallocatedList( _Descriptor* node_to_insert);
	void AddToFreeList(_Descriptor* node_to_insert);
	void CombineBlocks(_Descriptor* first, _Descriptor* second);
	void NullRootReference(_Descriptor* node_to_null);
	void MoveRootReferencesForward(_Descriptor* node_to_move_forward);

	bool CheckGuardBands(_Descriptor* node_to_check);
	bool IsPowerOfTwo(const uint8_t input);


	_Descriptor * SearchForBlock(const void * addr_to_search_for, _Descriptor* root_node) const;
	_Descriptor * FindSuitableUnallocatedBlock(const size_t amt) const;
	_Descriptor * RemoveBlockFromList(const void* addr_to_search_for, _Descriptor* root_node);
	_Descriptor * StealFromBlock(_Descriptor* victim, const size_t amt_to_take, const uint8_t alignment);





#pragma region AllocatorMemberVars
	_Descriptor * free_root_ = 0;
	_Descriptor * allocated_root_ = 0;
	_Descriptor * unallocated_root_ = 0;
	_Descriptor * front_of_pool_ = 0;

	void * front_of_chunk_;
	void * back_of_chunk_;
#pragma endregion AllocatorMemberVars

#pragma region Singleton
	static GAllocator* singleton_instance_;
	static void * singleton_instance_addr_;


	

#pragma endregion Singleton
};

