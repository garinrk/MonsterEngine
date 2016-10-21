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
	MonsterAllocator();
	~MonsterAllocator();
	char * MonsterMalloc(size_t amt);
	void MonsterFree(void * addr);
	void GarbageCollect();
	size_t bytesLeft;

private:
	void InitializeFreeList();
	void AddToAllocated(BlockDescriptor* toInsert);
	void AddToUnallocated(BlockDescriptor* toInsert);
	void AddToFree(BlockDescriptor * toAdd);

	void ConsolidateBlocks(BlockDescriptor* first, BlockDescriptor* second);

	BlockDescriptor * SearchForBlock(void * baseAddr);
	BlockDescriptor * RemoveFromList(void * addr, BlockDescriptor * root);

	void PrintLists();
	
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





