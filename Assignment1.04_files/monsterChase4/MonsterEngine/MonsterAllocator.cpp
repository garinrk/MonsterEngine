#include "MonsterAllocator.h"
#include "MonsterDebug.h"

#include <malloc.h>
#include <stdio.h>
#include <assert.h>
#define TOTALHEAPSIZE 8192
#define ALIGNMENT 4
#define NUMBEROFDESCRIPTORS 64

MonsterAllocator::MonsterAllocator()
{
	frontOfChunk = (char*) _aligned_malloc(TOTALHEAPSIZE, ALIGNMENT);

	assert(frontOfChunk != NULL && "NULL Memory Allocation");

	backOfChunk = frontOfChunk + TOTALHEAPSIZE;

	InitializeFreeList();

#ifdef _DEBUG
	PrintLists();
#endif // _DEBUG


}

MonsterAllocator::~MonsterAllocator()
{
}

char * MonsterAllocator::MonsterMalloc(size_t amt) {
	char * result;



	//check for amount of memory 
	if (amt > totalBytes) {
		return NULL;
	}

	//no more free block descriptors, attempt to get some by garbage collecting.
	if (endOfFree == NULL) {
		GarbageCollect();
	}
	BlockDescriptor * sufficientBlock = FindSuitableUnallocBlock(amt);

	//if even after garbage collecting we don't have a descriptor.
	if (sufficientBlock == NULL)
		return nullptr;

	BlockDescriptor * newBD = StealFromBlock(sufficientBlock,amt);
	AddToAllocated(newBD);
	result = (char*)newBD->blockBase;


	DEBUGLOG("USER REQUEST %zu BYTES", amt);
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
		BlockDescriptor * foundBlock = UnallocBlockSearch(addrToSearchFor,unallocatedRoot);

		if (foundBlock == NULL) {
			placeHolder = placeHolder->next;
		}
		else {
			//we found a block right after, combine them.
			ConsolidateBlocks(foundBlock, placeHolder);
		}
		
		if (placeHolder == NULL )
			finished = true;
	}


}


BlockDescriptor * MonsterAllocator::UnallocBlockSearch(void * baseAddr, BlockDescriptor * start)
{
	BlockDescriptor * conductor;
	conductor = start;

	while (conductor!= NULL) {
		if (conductor->blockBase == baseAddr)
			return conductor;
		else
			conductor = conductor->next;
	}
	
	return nullptr;
}

BlockDescriptor * MonsterAllocator::FindSuitableUnallocBlock(size_t amt)
{
	BlockDescriptor * conductor;
	conductor = unallocatedRoot;

	while (conductor != NULL) {
		if (conductor->sizeOfBlock > amt) {
			return conductor;
		}
	}

	return nullptr;
}

void MonsterAllocator::ConsolidateBlocks(BlockDescriptor * first, BlockDescriptor * second)
{
	DEBUGLOG("Combining blocks %d and %d", first->id, second->id);
	size_t newSize = first->sizeOfBlock + second->sizeOfBlock;
	
	second->sizeOfBlock = newSize;
	RemoveFromList(first->blockBase, unallocatedRoot);
	AddToFree(first);

	


#ifdef _DEBUG
	PrintLists();
#endif // _DEBUG


}


void MonsterAllocator::AddToFree(BlockDescriptor * toAdd)
{
	toAdd->blockBase = NULL;
	toAdd->sizeOfBlock = NULL;
	endOfFree->next = toAdd;
	toAdd->prev = endOfFree;

	endOfFree = toAdd;
	endOfFree->next = NULL;
}

BlockDescriptor * MonsterAllocator::RemoveFromList(void * addr, BlockDescriptor * root)
{
	BlockDescriptor * conductor;
	conductor = root;

	while (conductor != NULL) {
		if (conductor->blockBase == addr) //found the block
		{
			//head case with something after it
			if (conductor->prev == NULL && conductor->next != NULL) {
				if (conductor == freeRoot) {
					freeRoot = conductor->next;
				}
				else if (conductor == allocatedRoot) {
					allocatedRoot = conductor->next;
				}
				else if (conductor == unallocatedRoot) {
					unallocatedRoot = conductor->next;
				}

				conductor->next->prev = NULL;
				conductor->next = NULL;


			}
			//root case
			else if (conductor->prev == NULL && conductor->next == NULL) {
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
				conductor->prev = NULL;

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

BlockDescriptor * MonsterAllocator::StealFromBlock(BlockDescriptor * victim, size_t amt)
{
	//get a bd from the free list
	BlockDescriptor * thief = endOfFree;
	endOfFree = endOfFree->prev;
	endOfFree->next = NULL;
	thief->prev = NULL;
	thief->blockBase = victim->blockBase; //we're going to take the front of the victim space
	thief->sizeOfBlock = amt;

	victim->sizeOfBlock -= amt;
	char * modifiedBlockBase = (char*)victim->blockBase + amt;
	victim->blockBase = modifiedBlockBase;



	return thief;
}

void MonsterAllocator::PrintLists()
{
	//print free lists
	DEBUGLOG("=========PRINT START");

	BlockDescriptor * conductor;
	conductor = freeRoot;


	while (conductor != NULL) {
		DEBUGLOG("FREE NODE [addr:0x%04x id:%d]: prev:0x%04x\tblockptr:%04x\tsize:%zd\tnext:0x%04x", conductor, conductor->id, conductor->prev,conductor->blockBase,conductor->sizeOfBlock,conductor->next);
		conductor = conductor->next;
	}

	conductor = allocatedRoot;

	while (conductor != NULL) {
		DEBUGLOG("ALLOC NODE [addr:0x%04x id:%d]: prev:0x%04x\tblockptr:%04x\tsize:%zd\tnext:0x%04x", conductor, conductor->id, conductor->prev, conductor->blockBase, conductor->sizeOfBlock, conductor->next);
		conductor = conductor->next;
	}

	conductor = unallocatedRoot;

	while (conductor != NULL) {
		DEBUGLOG("UNALLOC NODE [addr:0x%04x id:%d]: prev:0x%04x\tblockptr:%04x\tsize:%zd\tnext:0x%04x", conductor, conductor->id, conductor->prev, conductor->blockBase, conductor->sizeOfBlock, conductor->next);
		conductor = conductor->next;
	}

	DEBUGLOG("=========PRINT END");

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
	DEBUGLOG("==USER FREE: %04X BEFORE GARBAGE COLLECT==", addr);
#ifdef _DEBUG
	PrintLists();
#endif // _DEBUG

	GarbageCollect();
	DEBUGLOG("==AFTER GARBAGE COLLECT==", addr);
#ifdef _DEBUG
	PrintLists();
#endif // _DEBUG
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

	totalBytes = (size_t)(frontOfBD - (BlockDescriptor*)frontOfChunk);
	totalBytes = totalBytes * 32;

	//set up initial unallocated block
	BlockDescriptor * initialUnallocatedBlock;
	initialUnallocatedBlock = endOfFree; //use the end of free
	endOfFree = endOfFree->prev;
	endOfFree->next = NULL; //update end of free

	initialUnallocatedBlock->prev = NULL;
	initialUnallocatedBlock->blockBase = frontOfChunk;
	initialUnallocatedBlock->sizeOfBlock = totalBytes;
	initialUnallocatedBlock->next = NULL;

	AddToUnallocated(initialUnallocatedBlock);


}

