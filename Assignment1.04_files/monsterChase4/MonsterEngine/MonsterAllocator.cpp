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

/*
	allocated->prev =  NULL;
	unallocated->prev = NULL;*/

	InitializeFreeList();
	




}

MonsterAllocator::~MonsterAllocator()
{
}

char * MonsterAllocator::MonsterMalloc(size_t amt) {
	char * result;

	if (free->next != NULL) {
		BlockDescriptor* usersBD = free;
		
	}



	return result;
}

void MonsterAllocator::InitializeFreeList()
{
	frontOfBD = (BlockDescriptor*)(backOfChunk - (sizeof(BlockDescriptor) * NUMBEROFDESCRIPTORS));
	
	frontOfBD->prev = NULL;
	frontOfBD->blockBase = NULL;
	frontOfBD->sizeOfBlock = 0;

	BlockDescriptor * current = frontOfBD +1;
	current->blockBase = NULL;
	current->sizeOfBlock = 0;
	current->prev = frontOfBD;

	frontOfBD->next = current;
	

	
	
	for (int i = 0; i < NUMBEROFDESCRIPTORS-2; i++) {
		BlockDescriptor * newBD = current+1;
		if (newBD >= (BlockDescriptor*)backOfChunk)
			break;
		current->next = newBD;
		newBD->prev = current;
		newBD->blockBase = NULL;
		newBD->sizeOfBlock = 0;
		
		current = newBD;
	}

	free = frontOfBD;

	bytesLeft = (size_t)(frontOfBD - (BlockDescriptor*)frontOfChunk);
	bytesLeft = bytesLeft * 32;
}
