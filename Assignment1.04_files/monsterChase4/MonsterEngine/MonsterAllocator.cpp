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

	InitializeFreeList();

}

MonsterAllocator::~MonsterAllocator()
{
}

char * MonsterAllocator::MonsterMalloc(size_t amt) {
	char * result;


	assert(amt <= bytesLeft && "You ran out of memory!");
	assert(endOfFree != NULL && "No more descriptors!");

	//check for amount of memory 
	if (amt > bytesLeft) {
		return NULL;
	}

	//OLD, PREVIOUS CHECK WAS IF WE RAN OUT OF FREE BLOCK DESCRIPTORS
	//if (endOfFree == NULL) {
	//	return NULL;
	//}



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
	
	//if the only thing left is the free-root node
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

#ifdef _DEBUG
	PrintLists();
#endif


	return result;
}

void MonsterAllocator::AddToAllocated(BlockDescriptor * toInsert)
{
	BlockDescriptor* conductor;
	conductor = allocatedRoot;

	if (allocatedRoot == 0) {
		toInsert->prev = NULL;
		toInsert->next = NULL;
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
		unallocatedRoot->prev = NULL;
		unallocatedRoot->next = NULL;
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

void MonsterAllocator::GarbageCollect()
{
	//looks through the list of free memory blocks
	//combing any that can be combined into a single larger free block.

	BlockDescriptor * placeHolder;
	placeHolder = unallocatedRoot;

	bool finished = false;

	if (unallocatedRoot == 0)
		return;

	while (!finished) {

		char * addrToSearchFor;
		addrToSearchFor = static_cast<char*>(placeHolder->blockBase) + placeHolder->sizeOfBlock;
		BlockDescriptor * foundBlock = SearchForBlock(addrToSearchFor);

		if (foundBlock == NULL) {
			placeHolder = placeHolder->next;
		}
		else {
			//we found a block right after, combine them.
			ConsolidateBlocks(placeHolder, foundBlock);
		}
		
		if (placeHolder == NULL )
			finished = true;
	}

	

}


BlockDescriptor * MonsterAllocator::SearchForBlock(void * baseAddr)
{
	BlockDescriptor * conductor;
	conductor = unallocatedRoot;

	while (conductor!= NULL) {
		if (conductor->blockBase == baseAddr)
			return conductor;
		else
			conductor = conductor->next;
	}
	
	return nullptr;
}

void MonsterAllocator::ConsolidateBlocks(BlockDescriptor * first, BlockDescriptor * second)
{
	size_t newSize = first->sizeOfBlock + second->sizeOfBlock;
	first->sizeOfBlock = newSize;
	//remove the second from the list and put back in the free list

	RemoveFromList(second->blockBase, unallocatedRoot);
	AddToFree(second);

}


void MonsterAllocator::AddToFree(BlockDescriptor * toAdd)
{
	endOfFree->next = toAdd;
	toAdd->prev = endOfFree;

	endOfFree = toAdd;
}

BlockDescriptor * MonsterAllocator::RemoveFromList(void * addr, BlockDescriptor * root)
{
	BlockDescriptor * conductor;
	conductor = root;

	while (conductor != NULL) {
		if (conductor->blockBase == addr) //found the block
		{
			//head case
			if (conductor->prev == NULL && conductor->next != NULL) {
				conductor->next->prev = NULL;
				conductor->next = NULL;

				if (conductor == freeRoot) {
					freeRoot = NULL;
				}
				else if (conductor == allocatedRoot) {
					allocatedRoot = NULL;
				}
				else if (conductor == unallocatedRoot) {
					unallocatedRoot = NULL;
				}
			}
			//middle
			else if (conductor->prev != NULL && conductor->next != NULL) {
				conductor->next->prev = conductor->prev;
				conductor->prev->next = conductor->next;
				conductor->next = NULL;
				conductor->prev = NULL;
			} 
			//tail
			else if (conductor->prev != NULL && conductor->next == NULL) {
				conductor->prev->next = NULL;
				
				if (conductor == endOfFree) {
					endOfFree = NULL;
				}
			}

			return conductor;

		}//if

		else {
			//move on
			conductor = conductor->next;
		}
	}//while

	return nullptr;
}

void MonsterAllocator::PrintLists()
{
	//print free lists
	DEBUGLOG("START");
	DEBUGLOG("FREE");
	BlockDescriptor * conductor;
	conductor = freeRoot;


	while (conductor != NULL) {
		DEBUGLOG("FREE NODE: %d with ptr %p and size %zd", conductor->id, conductor->blockBase, conductor->sizeOfBlock);
		conductor = conductor->next;
	}

	conductor = allocatedRoot;

	while (conductor != NULL) {
		DEBUGLOG("ALLOC NODE: %d with ptr %p and size %zd", conductor->id, conductor->blockBase, conductor->sizeOfBlock);
		conductor = conductor->next;
	}

	conductor = unallocatedRoot;

	while (conductor != NULL) {
		DEBUGLOG("UNALLOC NODE: %d with ptr %p and size %zd", conductor->id, conductor->blockBase, conductor->sizeOfBlock);
		conductor = conductor->next;
	}

}


void MonsterAllocator::MonsterFree(void * addr)
{
	BlockDescriptor * toMoveToUnallocated = RemoveFromList(addr, allocatedRoot);

	assert(toMoveToUnallocated != NULL && "Attempted to free a non valid addr");
	//if they free things that don't exist in the allocated list
	if (toMoveToUnallocated == NULL) {
		return;
	}
	AddToUnallocated(toMoveToUnallocated);
	GarbageCollect();
}







void MonsterAllocator::InitializeFreeList()
{
	frontOfBD = (BlockDescriptor*)(backOfChunk - (sizeof(BlockDescriptor) * NUMBEROFDESCRIPTORS));
	
	frontOfBD->prev = NULL;
	frontOfBD->blockBase = NULL;
	frontOfBD->sizeOfBlock = 0;\
	
	BlockDescriptor * current;
	current = frontOfBD;

#ifdef _DEBUG
	current->id = 0;
#endif
	
	for (int i = 0; i < NUMBEROFDESCRIPTORS-1; i++) {
		BlockDescriptor * newBD = current+1;
		if (newBD >= (BlockDescriptor*)backOfChunk)
			break;
		current->next = newBD;
		newBD->prev = current;
		newBD->blockBase = NULL;
		newBD->sizeOfBlock = 0;
		newBD->next = NULL;

#ifdef _DEBUG
		newBD->id = i + 1;
#endif

		current = newBD;
	}

	freeRoot = frontOfBD;
	endOfFree = current;

	bytesLeft = (size_t)(frontOfBD - (BlockDescriptor*)frontOfChunk);
	bytesLeft = bytesLeft * 32;

#ifdef _DEBUG
	PrintLists();
#endif
}

