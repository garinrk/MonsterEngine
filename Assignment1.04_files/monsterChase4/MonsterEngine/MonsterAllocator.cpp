#include "MonsterAllocator.h"
#include "MonsterDebug.h"

#include <malloc.h>
#include <stdio.h>
#include <assert.h>
#define HEAPSIZE 4096
#define ALIGNMENT 4
#define NUMBEROFDESCRIPTORS 64

MonsterAllocator::MonsterAllocator()
{
	frontOfChunk = (char*) _aligned_malloc(HEAPSIZE, ALIGNMENT);

	assert(frontOfChunk != NULL && "NULL Memory Allocation");

	backOfChunk = frontOfChunk + HEAPSIZE;

	//allocated = NULL;
	//unallocated = NULL;

	InitializeFreeList();
	




}

MonsterAllocator::~MonsterAllocator()
{
}

char * MonsterAllocator::MonsterMalloc(size_t amt) {
	char * result;



	return result;
}

void MonsterAllocator::InitializeFreeList()
{
	BlockDescriptor * frontOfBlockDescriptor = (BlockDescriptor*)(backOfChunk - (sizeof(BlockDescriptor) * NUMBEROFDESCRIPTORS));

	
	frontOfBlockDescriptor->blockBase = NULL;
	frontOfBlockDescriptor->sizeOfBlock = 0;

	BlockDescriptor * current = frontOfBlockDescriptor+1;
	current->blockBase = NULL;
	current->sizeOfBlock = 0;

	frontOfBlockDescriptor->next = current;
	
	
	for (int i = 0; i < NUMBEROFDESCRIPTORS-2; i++) {
		BlockDescriptor * newBD = current+1;
		if (newBD >= (BlockDescriptor*)backOfChunk)
			break;
		current->next = newBD;
		newBD->blockBase = NULL;
		newBD->sizeOfBlock = 0;
		
		current = newBD;
	}

	free = frontOfBlockDescriptor;
}
