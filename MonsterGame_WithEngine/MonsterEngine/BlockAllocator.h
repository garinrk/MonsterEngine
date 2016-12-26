#pragma once
struct BlockDescriptor {
	BlockDescriptor * prev;
	void * base;
	void * user_ptr;
	size_t master_size;
	size_t user_size;
#if _DEBUG
	int debug_id;
#endif
	BlockDescriptor * next;
};


#include "MonsterDebug.h"

#include <assert.h>
#include <inttypes.h>
#include <malloc.h>
#include <new>

#define BAND_VAL 0xFD
#define BAND_SIZE 4

#define DEFAULT_ALIGNMENT 4
#define DEFAULT_TOTAL_SIZE 1024*1024*100 // 100mb
#define DEFAULT_NUM_DESCRIPTORS 5192

namespace MMEngine {
	class BlockAllocator
	{
	public:

		BlockAllocator(const size_t i_totalSizeOfAllocator, const unsigned int i_amtOfDescriptors, const uint8_t i_align);

		void * GAlloc(const size_t i_amt);
		void * GAlloc(const size_t i_amt, const uint8_t i_align);

		bool GFree(const void* addr_to_free);
		void GGCollect();

		static void CreateInstance(const size_t i_totalSizeOfAllocator, const unsigned int i_amtOfDescriptors, const uint8_t i_align);
		static void CreateInstance(); //default using defined sizes
		static BlockAllocator* GetInstance();
		static void DestroyInstance();

		~BlockAllocator();

		size_t total_bytes_left = 0;

		void PrintList(BlockDescriptor* i_root);

		//For unit tests
		bool ContainsAddressInBlock(const void* i_addr);
		bool IsAllocatedAddress(const void* i_addr);
		size_t GetLargestFreeBlockSize();

		//debug
		void PrintAllocatorState();
	private:

		void InitializeFreeList(const unsigned int i_amtOfDescriptors);
		void AddToAllocatedList(BlockDescriptor* i_node);
		void AddToUnallocatedList(BlockDescriptor* i_node);
		void AddToFreeList(BlockDescriptor* i_node);
		void CombineBlocks(BlockDescriptor* i_firstBlock, BlockDescriptor* i_secondBlock);
		void NullRootReference(BlockDescriptor* i_node);
		void MoveRootReferencesForward(BlockDescriptor* i_node);

		bool CheckGuardBands(BlockDescriptor* i_node);
		bool IsPowerOfTwo(const uint8_t i_toCheck);


		BlockDescriptor * SearchForBlock(const void * i_addr, BlockDescriptor* i_root) const;
		BlockDescriptor * FindSuitableUnallocatedBlock(const size_t i_amt) const;
		BlockDescriptor * RemoveBlockFromList(const void* i_addr, BlockDescriptor* i_root);
		BlockDescriptor * StealFromBlock(BlockDescriptor* i_victim, const size_t i_amtToTake, const uint8_t i_align);
		BlockDescriptor * RemoveBlockFromListByBase(const void* i_addr, BlockDescriptor * i_root);





#pragma region AllocatorMemberVars
		BlockDescriptor * free_root_ = 0;
		BlockDescriptor * allocated_root_ = 0;
		BlockDescriptor * unallocated_root_ = 0;
		BlockDescriptor * front_of_pool_ = 0;

		void * front_of_chunk_;
		void * back_of_chunk_;
#pragma endregion AllocatorMemberVars

#pragma region Singleton
		static BlockAllocator* singleton_instance_;
		static void * singleton_instance_addr_;




#pragma endregion Singleton
	};

}