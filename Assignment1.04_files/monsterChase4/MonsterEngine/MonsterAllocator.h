#pragma once

class MonsterAllocator
{

struct BlockDescriptor {
	BlockDescriptor * prev;
	void * blockBase;
	size_t sizeOfBlock;
	BlockDescriptor * next;
	
};

public:
	MonsterAllocator();
	~MonsterAllocator();
	char * MonsterMalloc(size_t amt);
	void AddToAllocated(BlockDescriptor* toInsert);
	void AddToUnallocated(BlockDescriptor* toInsert);
	char * frontOfChunk;
	char * backOfChunk;

	BlockDescriptor * freeRoot = 0;
	BlockDescriptor * endOfFree = 0;
	BlockDescriptor * allocatedRoot = 0;
	BlockDescriptor * unallocatedRoot = 0;
	BlockDescriptor * frontOfBD;

	size_t bytesLeft;

private:
	void InitializeFreeList();

};





