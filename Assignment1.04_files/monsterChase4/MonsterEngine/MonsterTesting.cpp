#include "MonsterTesting.h"
#include "MonsterAllocator.h"
#include "MonsterDebug.h"
#include <assert.h>
#include <algorithm>
#include <vector>
#define TEST_SINGLE_LARGE_ALLOCATION
//#define __TRACK_ALLOCATIONS

bool MonsterTesting::RunTests()
{

	const size_t 		sizeHeap = 1024;
	const unsigned int 	numDescriptors = 64;
	const size_t		align = 4;
	// Allocate memory for my test heap.
	//void * pHeapMemory = _aligned_malloc(sizeHeap, 4);
	//assert(pHeapMemory);

	// Create a heap manager for my test heap.
	MonsterAllocator pHeapManager = MonsterAllocator(sizeHeap, numDescriptors, align);

#ifdef TEST_SINGLE_LARGE_ALLOCATION
	// This is a test I wrote to check to see if using the whole block if it was almost consumed by 
	// an allocation worked. Also helped test my ShowFreeBlocks() and ShowOutstandingAllocations().
	{
		//ShowFreeBlocks(pHeapManager);

		//size_t largestBeforeAlloc = GetLargestFreeBlock(pHeapManager);
		size_t largestBeforeAlloc = pHeapManager.GetLargestFreeBlock();
		//void * pPtr = alloc(pHeapManager, largestBeforeAlloc - HeapManager::s_MinumumToLeave);
		//void * pPtr = alloc(pHeapManager, largestBeforeAlloc - sizeof(BlockDescriptor));
		void * pPtr = pHeapManager.MonsterMalloc(largestBeforeAlloc);

		if (pPtr)
		{
			//ShowFreeBlocks(pHeapManager);
			//printf("\n");
#ifdef __TRACK_ALLOCATIONS
			ShowOutstandingAllocations(pHeapManager);
#endif // __TRACK_ALLOCATIONS
			//printf("\n");

			//size_t largestAfterAlloc = GgetLargestFreeBlock(pHeapManager);
			size_t largestAfterAlloc = pHeapManager.GetLargestFreeBlock();
			//free(pHeapManager, pPtr);
			pHeapManager.MonsterFree(pPtr);

			//ShowFreeBlocks(pHeapManager);
#ifdef __TRACK_ALLOCATIONS
			ShowOutstandingAllocations(pHeapManager);
#endif // __TRACK_ALLOCATIONS
			//printf("\n");

			//Collect(pHeapManager);
			pHeapManager.GarbageCollect();

			//ShowFreeBlocks(pHeapManager);
#ifdef __TRACK_ALLOCATIONS
			ShowOutstandingAllocations(pHeapManager);
#endif // __TRACK_ALLOCATIONS
			//printf("\n");

			size_t largestAfterCollect = pHeapManager.GetLargestFreeBlock();
		}
	}
#endif

	return true;
	std::vector<void *> AllocatedAddresses;

	long	numAllocs = 0;
	long	numFrees = 0;
	long	numCollects = 0;

	bool	done = false;

	// allocate memory of random sizes up to 1024 bytes from the heap manager
	// until it runs out of memory
	do
	{
		const size_t		maxTestAllocationSize = 1024;

		//const unsigned int	alignments[] = { 4, 8, 16, 32, 64 };

		//unsigned int	index = rand() % (sizeof(alignments) / sizeof(alignments[0]));

		//unsigned int	alignment = alignments[index];

		size_t			sizeAlloc = 1 + (rand() & (maxTestAllocationSize - 1));

		//void * pPtr = alloc(pHeapManager, sizeAlloc, alignment);
		void * pPtr = pHeapManager.MonsterMalloc(sizeAlloc);

		//assert((reinterpret_cast<uintptr_t>(pPtr) & (alignment - 1)) == 0);

		if (pPtr == NULL)
		{
			pHeapManager.GarbageCollect();
			//Collect(pHeapManager);

			//pPtr = alloc(pHeapManager, sizeAlloc, alignment);
			void * pPtr = pHeapManager.MonsterMalloc(sizeAlloc);

			
			if (pPtr == NULL)
			{
				done = true;
				break;
			}
		}

		pHeapManager.PrintLists();

		AllocatedAddresses.push_back(pPtr);
		numAllocs++;

		const unsigned int freeAboutEvery = 10;
		const unsigned int garbageCollectAboutEvery = 40;

		if (!AllocatedAddresses.empty() && ((rand() % freeAboutEvery) == 0))
		{
			void * pPtr = AllocatedAddresses.back();
			AllocatedAddresses.pop_back();

			//bool success = free(pHeapManager, pPtr);
			bool success = pHeapManager.MonsterFree(pPtr);
			assert(success);

			numFrees++;
		}

		if ((rand() % garbageCollectAboutEvery) == 0)
		{
			//Collect(pHeapManager);
			pHeapManager.GarbageCollect();

			numCollects++;
		}

	} while (1);

	//printf("After exhausting allocations:\n");
	pHeapManager.PrintAllocatedList();
	//ShowFreeBlocks(pHeapManager);
	pHeapManager.PrintUnallocatedList();

#ifdef __TRACK_ALLOCATIONS
	//ShowOutstandingAllocations(pHeapManager);
	pHeapManager.PrintAllocatedList();
#endif // __TRACK_ALLOCATIONS


	// now free those blocks in a random order
	if (!AllocatedAddresses.empty())
	{
		// randomize the addresses
		std::random_shuffle(AllocatedAddresses.begin(), AllocatedAddresses.end());

		// return them back to the heap manager
		while (!AllocatedAddresses.empty())
		{
			void * pPtr = AllocatedAddresses.back();
			AllocatedAddresses.pop_back();

			//bool success = Contains(pHeapManager, pPtr); //if in the block in general
			bool success = pHeapManager.Contains(pPtr);
			assert(success);

			//success = IsAllocated(pHeapManager, pPtr);
			success = pHeapManager.isAllocated(pPtr);
			assert(success);

			//success = free(pHeapManager, pPtr);
			success = pHeapManager.MonsterFree(pPtr);
			assert(success);
		}


		//ShowFreeBlocks(pHeapManager);
		pHeapManager.PrintFreeList();
#ifdef __TRACK_ALLOCATIONS
		//ShowOutstandingAllocations(pHeapManager);
		pHeapManager.PrintAllocatedList();
#endif // __TRACK_ALLOCATIONS

		// do garbage collection
		//Collect(pHeapManager);
		pHeapManager.GarbageCollect();

		pHeapManager.PrintLists();
		// our heap should be one single block, all the memory it started with


		//ShowFreeBlocks(pHeapManager);
		pHeapManager.PrintFreeList();
#ifdef __TRACK_ALLOCATIONS
		//ShowOutstandingAllocations(pHeapManager);
		pHeapManager.PrintAllocatedList();
#endif // __TRACK_ALLOCATIONS

	// do a large test allocation to see if garbage collection worked
		//void * pPtr = alloc(pHeapManager, sizeHeap / 2);
		void * pPtr = pHeapManager.MonsterMalloc(sizeHeap / 2);
		assert(pPtr);

		if (pPtr)
		{
			//bool success = free(pHeapManager, pPtr);
			bool success = pHeapManager.MonsterFree(pPtr);
			assert(success);

		}
	}

	//Destroy(pHeapManager);
	//pHeapManager = NULL;

	//_aligned_free(pHeapMemory);

	// we succeeded
	return true;
}
