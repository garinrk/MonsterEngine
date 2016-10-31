#include "MonsterTesting.h"
#include "MonsterAllocator.h"
#include "MonsterDebug.h"
#include <assert.h>
#include <algorithm>
#include <vector>
//#define TEST_SINGLE_LARGE_ALLOCATION
//#define __TRACK_ALLOCATIONS

bool MonsterTesting::RunAllocatorTests()
{

	const size_t 		sizeHeap = 1024;
	const unsigned int 	numDescriptors =  8;
	const size_t		align = 3;

	// Create a heap manager for my test heap.
	MonsterAllocator pHeapManager = MonsterAllocator(sizeHeap, numDescriptors, align);

#ifdef TEST_SINGLE_LARGE_ALLOCATION
	// This is a test I wrote to check to see if using the whole block if it was almost consumed by 
	// an allocation worked. Also helped test my ShowFreeBlocks() and ShowOutstandingAllocations().
	{
		//ShowFreeBlocks(pHeapManager);

		size_t largestBeforeAlloc = pHeapManager.GetLargestFreeBlock();
		void * pPtr = pHeapManager.MonsterMalloc(largestBeforeAlloc);

		if (pPtr)
		{
#ifdef __TRACK_ALLOCATIONS
			pHeapManager.PrintLists();
#endif // __TRACK_ALLOCATIONS
			size_t largestAfterAlloc = pHeapManager.GetLargestFreeBlock();
			pHeapManager.MonsterFree(pPtr);

			//ShowFreeBlocks(pHeapManager);
#ifdef __TRACK_ALLOCATIONS
			pHeapManager.PrintLists();
#endif // __TRACK_ALLOCATIONS
			pHeapManager.GarbageCollect();

			//ShowFreeBlocks(pHeapManager);
#ifdef __TRACK_ALLOCATIONS
			pHeapManager.PrintLists();
#endif // __TRACK_ALLOCATIONS

			size_t largestAfterCollect = pHeapManager.GetLargestFreeBlock();
		}
	}
#endif

	std::vector<void *> AllocatedAddresses;

	long	numAllocs = 0;
	long	numFrees = 0;
	long	numCollects = 0;

	bool	done = false;

	// allocate memory of random sizes up to 1024 bytes from the heap manager
	// until it runs out of memory
	do
	{
		const size_t maxTestAllocationSize = 512;


		size_t	sizeAlloc = 1 + (rand() & (maxTestAllocationSize - 1));

		void * pPtr = pHeapManager.MonsterMalloc(sizeAlloc);
		pHeapManager.PrintLists();

		if (pPtr == NULL)
		{
			pHeapManager.GarbageCollect();

			void * pPtr = pHeapManager.MonsterMalloc(sizeAlloc);

			
			if (pPtr == NULL)
			{
				done = true;
				break;
			}
		}

#ifdef __TRACK_ALLOCATIONS
		pHeapManager.PrintLists();
#endif // __TRACK_ALLOCATIONS

		AllocatedAddresses.push_back(pPtr);
		numAllocs++;

		const unsigned int freeAboutEvery = 10;
		const unsigned int garbageCollectAboutEvery = 40;

		if (!AllocatedAddresses.empty() && ((rand() % freeAboutEvery) == 0))
		{
			void * pPtr = AllocatedAddresses.back();
			AllocatedAddresses.pop_back();

			bool success = pHeapManager.MonsterFree(pPtr);
			assert(success);

			numFrees++;
		}

		if ((rand() % garbageCollectAboutEvery) == 0)
		{

			pHeapManager.GarbageCollect();

			numCollects++;
		}

	} while (1);


#ifdef __TRACK_ALLOCATIONS
	pHeapManager.PrintLists();
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

			bool success = pHeapManager.Contains(pPtr);
			assert(success);


			success = pHeapManager.isAllocated(pPtr);
			assert(success);

			success = pHeapManager.MonsterFree(pPtr);
			assert(success);
		}

		
#ifdef __TRACK_ALLOCATIONS
		pHeapManager.PrintLists();
#endif // __TRACK_ALLOCATIONS

	
		// our heap should be one single block, all the memory it started with


#ifdef __TRACK_ALLOCATIONS
		pHeapManager.PrintLists();
#endif // __TRACK_ALLOCATIONS

	// do a large test allocation to see if garbage collection worked
		void * pPtr = pHeapManager.MonsterMalloc(sizeHeap / 2);
		assert(pPtr);

		if (pPtr)
		{
			bool success = pHeapManager.MonsterFree(pPtr);
			assert(success);

		}
	}

	return true;
}
