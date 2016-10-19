#pragma once
struct BlockDescriptor {
	BlockDescriptor * prev;
	void * blockBase;
	size_t sizeOfBlock;
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
	BlockDescriptor * SearchForBlock(void * baseAddr);
	void ConsolidateBlocks(BlockDescriptor* first, BlockDescriptor* second);
	void RemoveFromUnallocated(BlockDescriptor * toRemove);
	BlockDescriptor * RemoveFromAllocated(void * addr);
	void AddToFree(BlockDescriptor * toAdd);
	
	char * frontOfChunk;
	char * backOfChunk;

	BlockDescriptor * freeRoot = 0;
	BlockDescriptor * endOfFree = 0;
	BlockDescriptor * allocatedRoot = 0;
	BlockDescriptor * unallocatedRoot = 0;
	BlockDescriptor * frontOfBD;


};





