#pragma once
class MonsterAllocator
{

struct BlockDescriptor {
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

private:
	void InitializeFreeList();

};





