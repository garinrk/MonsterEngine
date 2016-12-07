#include "MonsterTesting.h"
#include "MonsterAllocator.h"
#include "GAllocator.h"
#include "MonsterDebug.h"
#include <assert.h>
#include <algorithm>
#include <vector>
#define TEST_SINGLE_LARGE_ALLOCATION
//#define __TRACK_ALLOCATIONS

bool MonsterTesting::ModifiedAllocatorTests()
{

	const size_t 		sizeHeap = 1024;
	const unsigned int 	numDescriptors =  8;
	const uint8_t		initial_alignment = 4;
	// Create a heap manager for my test heap.
	MonsterAllocator pHeapManager = MonsterAllocator(sizeHeap, numDescriptors,initial_alignment);

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
		//pHeapManager.PrintLists();

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


bool MonsterTesting::OriginalAllocatorTests()
{

	const size_t 		sizeHeap = 1024 * 1024;
	const unsigned int 	numDescriptors = 2048;
	const uint8_t		initial_alignment = 4;
	// Allocate memory for my test heap.
	void * pHeapMemory = _aligned_malloc(sizeHeap, 4);
	assert(pHeapMemory);

	// Create a heap manager for my test heap.
	MonsterAllocator pHeapManager = MonsterAllocator(sizeHeap, numDescriptors, initial_alignment);
	

#ifdef TEST_SINGLE_LARGE_ALLOCATION
	// This is a test I wrote to check to see if using the whole block if it was almost consumed by 
	// an allocation worked. Also helped test my ShowFreeBlocks() and ShowOutstandingAllocations().
	{
		//ShowFreeBlocks(pHeapManager);

		size_t largestBeforeAlloc = pHeapManager.GetLargestFreeBlock();
		//void * pPtr = alloc(pHeapManager, largestBeforeAlloc - HeapManager::s_MinumumToLeave);
		void * pPtr = pHeapManager.MonsterMalloc(largestBeforeAlloc);

		if (pPtr)
		{
#ifdef __TRACK_ALLOCATIONS
			ShowOutstandingAllocations(pHeapManager);
#endif // __TRACK_ALLOCATIONS

			size_t largestAfterAlloc = pHeapManager.GetLargestFreeBlock();
			pHeapManager.MonsterFree(pPtr);

#ifdef __TRACK_ALLOCATIONS
			ShowOutstandingAllocations(pHeapManager);
#endif // __TRACK_ALLOCATIONS

			pHeapManager.GarbageCollect();

#ifdef __TRACK_ALLOCATIONS
			ShowOutstandingAllocations(pHeapManager);
#endif // __TRACK_ALLOCATIONS
			printf("\n");

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
		const size_t		maxTestAllocationSize = 1024;

		const unsigned int	alignments[] = { 4, 8, 16, 32, 64 };

		unsigned int	index = rand() % (sizeof(alignments) / sizeof(alignments[0]));

		unsigned int	alignment = alignments[index];

		size_t			sizeAlloc = 1 + (rand() & (maxTestAllocationSize - 1));

		//void * pPtr = alloc(pHeapManager, sizeAlloc, alignment);
		void * pPtr = pHeapManager.MonsterMalloc(sizeAlloc, alignment);

		assert((reinterpret_cast<uintptr_t>(pPtr) & (alignment - 1)) == 0);

		if (pPtr == NULL)
		{
			pHeapManager.GarbageCollect();

			pPtr = pHeapManager.MonsterMalloc(sizeAlloc, alignment);

			if (pPtr == NULL)
			{
				done = true;
				break;
			}
		}

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
	ShowOutstandingAllocations(pHeapManager);
#endif // __TRACK_ALLOCATIONS
	printf("\n");

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
		ShowOutstandingAllocations(pHeapManager);
#endif // __TRACK_ALLOCATIONS

		// do garbage collection
		pHeapManager.GarbageCollect();
		// our heap should be one single block, all the memory it started with


#ifdef __TRACK_ALLOCATIONS
		ShowOutstandingAllocations(pHeapManager);
#endif // __TRACK_ALLOCATIONS

		void * pPtr = pHeapManager.MonsterMalloc(sizeHeap / 2);
		assert(pPtr);

		if (pPtr)
		{
			bool success = pHeapManager.MonsterFree(pPtr);
			assert(success);

		}
	}
	
	_aligned_free(pHeapMemory);

	// we succeeded
	return true;
}

bool MonsterTesting::GAllocatorTests() {


	const size_t 		sizeHeap = 1024;
	const unsigned int 	numDescriptors = 8;
	const uint8_t		initial_alignment = 4;
	// Create a heap manager for my test heap.
	GAllocator pHeapManager = GAllocator(sizeHeap, numDescriptors, initial_alignment);

#ifdef TEST_SINGLE_LARGE_ALLOCATION
	// This is a test I wrote to check to see if using the whole block if it was almost consumed by 
	// an allocation worked. Also helped test my ShowFreeBlocks() and ShowOutstandingAllocations().
	{
		//ShowFreeBlocks(pHeapManager);

		size_t largestBeforeAlloc = pHeapManager.GetLargestFreeBlockSize();
		void * pPtr = pHeapManager.GAlloc(largestBeforeAlloc);

		if (pPtr)
		{
#ifdef __TRACK_ALLOCATIONS
			pHeapManager.PrintLists();
#endif // __TRACK_ALLOCATIONS
			size_t largestAfterAlloc = pHeapManager.GetLargestFreeBlockSize();
			pHeapManager.GFree(pPtr);

			//ShowFreeBlocks(pHeapManager);
#ifdef __TRACK_ALLOCATIONS
			pHeapManager.PrintLists();
#endif // __TRACK_ALLOCATIONS
			pHeapManager.GGCollect();

			//ShowFreeBlocks(pHeapManager);
#ifdef __TRACK_ALLOCATIONS
			pHeapManager.PrintLists();
#endif // __TRACK_ALLOCATIONS

			size_t largestAfterCollect = pHeapManager.GetLargestFreeBlockSize();
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

		void * pPtr = pHeapManager.GAlloc(sizeAlloc);
		//pHeapManager.PrintLists();

		if (pPtr == NULL)
		{
			pHeapManager.GGCollect();

			void * pPtr = pHeapManager.GAlloc(sizeAlloc);


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

			bool success = pHeapManager.GFree(pPtr);
			assert(success);

			numFrees++;
		}

		if ((rand() % garbageCollectAboutEvery) == 0)
		{

			pHeapManager.GGCollect();

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

			bool success = pHeapManager.ContainsAddress(pPtr);
			assert(success);


			success = pHeapManager.IsAllocatedAddress(pPtr);
			assert(success);

			success = pHeapManager.GFree(pPtr);
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
		void * pPtr = pHeapManager.GAlloc(sizeHeap / 2);
		assert(pPtr);

		if (pPtr)
		{
			bool success = pHeapManager.GFree(pPtr);
			assert(success);

		}
	}

	return true;

}

bool MonsterTesting::GAllocatorWithAlignmentTests() {
	return false;
}