#pragma once
struct _Descriptor {
	_Descriptor * prev;
	void * base;
	void * user_ptr;
	size_t master_size;
	size_t user_size;
#if _DEBUG
	int debug_id;
#endif
	_Descriptor * next;
};


#include "MonsterDebug.h"

#include <assert.h>
#include <inttypes.h>
#include <malloc.h>
#include <new>

#define BAND_VAL 0xFD
#define BAND_SIZE 4

#define DEFAULT_ALIGNMENT 4
#define DEFAULT_TOTAL_SIZE 1024*1024*100 // 100mb, to handle std::vector and up to 128 byte wide FSAs
#define DEFAULT_NUM_DESCRIPTORS 5192*2
class GAllocator
{
public:

	GAllocator(const size_t i_totalSizeOfAllocator, const unsigned int i_amtOfDescriptors, const uint8_t i_align );

	void * GAlloc(const size_t i_amt);
	void * GAlloc(const size_t i_amt, const uint8_t i_align);

	bool GFree(const void* addr_to_free);
	void GGCollect();

	static void CreateInstance(const size_t i_totalSizeOfAllocator, const unsigned int i_amtOfDescriptors, const uint8_t i_align);
	static void CreateInstance(); //default using defined sizes
	static GAllocator* GetInstance();
	static void DestroyInstance();

	~GAllocator();

	size_t total_bytes_left = 0;

	void PrintList(_Descriptor* i_root);

	//For unit tests
	bool ContainsAddressInBlock(const void* i_addr);
	bool IsAllocatedAddress(const void* i_addr);
	size_t GetLargestFreeBlockSize();

	//debug
	void PrintAllocatorState();
private:

	void InitializeFreeList(const unsigned int i_amtOfDescriptors);
	void AddToAllocatedList(_Descriptor* i_node);
	void AddToUnallocatedList( _Descriptor* i_node);
	void AddToFreeList(_Descriptor* i_node);
	void CombineBlocks(_Descriptor* i_firstBlock, _Descriptor* i_secondBlock);
	void NullRootReference(_Descriptor* i_node);
	void MoveRootReferencesForward(_Descriptor* i_node);

	bool CheckGuardBands(_Descriptor* i_node);
	bool IsPowerOfTwo(const uint8_t i_toCheck);


	_Descriptor * SearchForBlock(const void * i_addr, _Descriptor* i_root) const;
	_Descriptor * FindSuitableUnallocatedBlock(const size_t i_amt) const;
	_Descriptor * RemoveBlockFromList(const void* i_addr, _Descriptor* i_root);
	_Descriptor * StealFromBlock(_Descriptor* i_victim, const size_t i_amtToTake, const uint8_t i_align);





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

