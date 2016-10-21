#pragma once
struct BlockDescriptor {
	BlockDescriptor * prev;
	void * blockBase;
	size_t sizeOfBlock;


#if _DEBUG
	int id;
#endif // DEBUG

		BlockDescriptor * next;
};
class MonsterAllocator
{

public:
	MonsterAllocator(size_t sizeOfChunk, const unsigned int numDescriptors, size_t initialAlignment);
	~MonsterAllocator();
	void * MonsterMalloc(size_t amt);
	bool MonsterFree(void * addr);
	void GarbageCollect();
	size_t totalBytes;

	void PrintLists();
	void PrintAllocatedList();
	void PrintFreeList();
	void PrintUnallocatedList();
	bool Contains(void * addr);
	bool isAllocated(void * addr);

private:
	void InitializeFreeList(int numDescriptors);
	void AddToAllocated(BlockDescriptor* toInsert);
	void AddToUnallocated(BlockDescriptor* toInsert);
	void AddToFree(BlockDescriptor * toAdd);
	

	void ConsolidateBlocks(BlockDescriptor* first, BlockDescriptor* second);

	BlockDescriptor * UnallocBlockSearch(void * baseAddr, BlockDescriptor * start);
	BlockDescriptor * FindSuitableUnallocBlock(size_t amt);
	BlockDescriptor * RemoveFromList(void * addr, BlockDescriptor * root);
	BlockDescriptor * StealFromBlock(BlockDescriptor * victim, size_t amt);

	
	char * frontOfChunk;
	char * backOfChunk;

	BlockDescriptor * freeRoot = 0;
	BlockDescriptor * endOfFree = 0;
	BlockDescriptor * allocatedRoot = 0;
	BlockDescriptor * unallocatedRoot = 0;
	BlockDescriptor * frontOfBD;

	int num_free = 0;
	int num_alloc = 0;
	int num_unalloc = 0;


	


};





