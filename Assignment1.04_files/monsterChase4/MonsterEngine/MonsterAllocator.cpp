#include "MonsterAllocator.h"
#include "MonsterDebug.h"

#include <malloc.h>
#include <stdio.h>
#include <assert.h>
#define TOTALHEAPSIZE 4096
#define ALIGNMENT 4
#define NUMBEROFDESCRIPTORS 64

MonsterAllocator::MonsterAllocator()
{
	frontOfChunk = (char*) _aligned_malloc(TOTALHEAPSIZE, ALIGNMENT);

	assert(frontOfChunk != NULL && "NULL Memory Allocation");

	backOfChunk = frontOfChunk + TOTALHEAPSIZE;

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

	//check for amount of memory 
	assert(amt <= bytesLeft && "You ran out of memory!");
	assert(endOfFree != NULL && "No more descriptors!");

	//if there's still stuff left in the free list
	if (endOfFree->prev != NULL) {
		BlockDescriptor* newBD;
		newBD = endOfFree;
		endOfFree = endOfFree->prev;
		newBD->next = NULL;
		newBD->blockBase = frontOfChunk;
		newBD->sizeOfBlock = amt;
		frontOfChunk += amt;
		bytesLeft -= amt;
		AddToAllocated(newBD);
		result = (char*)newBD->blockBase;
	}
	
	//if the only thing left is the root node
	else if(endOfFree == freeRoot && freeRoot->blockBase == NULL) {
		BlockDescriptor * newBD;
		newBD = endOfFree;
		endOfFree = NULL;
		newBD->next = NULL;
		newBD->blockBase = frontOfChunk;
		newBD->sizeOfBlock = amt;
		frontOfChunk += amt;
		bytesLeft -= amt;
		AddToAllocated(newBD);
		result = (char*)newBD->blockBase;
	}

	//TODO: Free 
	//if there's no more block descriptors in the free list, we have to go look in the unallocated
	//list
	//if (endOfFree == NULL) {
	//	//go do that.
	//}




	return result;
}

void MonsterAllocator::AddToAllocated(BlockDescriptor * toInsert)
{
	BlockDescriptor* conductor;
	conductor = allocatedRoot;

	if (allocatedRoot == 0) {
		allocatedRoot = toInsert;
		return;
	}
	else {
		while (conductor->next != NULL) {
			conductor = conductor->next;
		}

		conductor->next = toInsert;
		toInsert->prev = conductor;
	}

}

void MonsterAllocator::AddToUnallocated(BlockDescriptor * toInsert)
{
	BlockDescriptor* conductor;
	conductor = unallocatedRoot;

	if (unallocatedRoot == 0) {
		unallocatedRoot = toInsert;
		return;
	}
	else {
		while (conductor->next != NULL) {
			conductor = conductor->next;
		}

		conductor->next = toInsert;
		toInsert->prev = conductor;
	}
}

void MonsterAllocator::InitializeFreeList()
{
	frontOfBD = (BlockDescriptor*)(backOfChunk - (sizeof(BlockDescriptor) * NUMBEROFDESCRIPTORS));
	
	frontOfBD->prev = NULL;
	frontOfBD->blockBase = NULL;
	frontOfBD->sizeOfBlock = 0;
/*
	BlockDescriptor * current = frontOfBD +1;
	current->blockBase = NULL;
	current->sizeOfBlock = 0;
	current->prev = frontOfBD;*/
	
	BlockDescriptor * current;
	current = frontOfBD;
	//frontOfBD->next = current;
	

	
	
	for (int i = 0; i < NUMBEROFDESCRIPTORS-1; i++) {
		BlockDescriptor * newBD = current+1;
		if (newBD >= (BlockDescriptor*)backOfChunk)
			break;
		current->next = newBD;
		newBD->prev = current;
		newBD->blockBase = NULL;
		newBD->sizeOfBlock = 0;
		newBD->next = NULL;
		
		current = newBD;
	}

	freeRoot = frontOfBD;
	endOfFree = current;

	bytesLeft = (size_t)(frontOfBD - (BlockDescriptor*)frontOfChunk);
	bytesLeft = bytesLeft * 32;
}
