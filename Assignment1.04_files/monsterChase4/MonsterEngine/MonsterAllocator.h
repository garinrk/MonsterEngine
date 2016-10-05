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
	char * frontOfChunk;
	char * backOfChunk;

	BlockDescriptor * free;
	BlockDescriptor * allocated;
	BlockDescriptor * unallocated;
	BlockDescriptor * frontOfBD;

	size_t bytesLeft;

private:
	void InitializeFreeList();

};





