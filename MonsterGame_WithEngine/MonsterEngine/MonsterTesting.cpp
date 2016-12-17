#include "MonsterTesting.h"
//#define TEST_SINGLE_LARGE_ALLOCATION

#define DEFAULT_BLOCK_ALLOCATOR_SIZE 1024*1024*50
#define DEFAULT_NUM_OF_DESCRIPTORS 5192
#define DEFAULT_NUM_OF_FSA_BLOCKS 64
#define DEFAULT_BLOCK_ALLOCATOR_ALIGNMENT 4

bool MonsterTesting::GAllocatorTests() {


	const size_t 		sizeHeap = 1024*1024;
	const unsigned int 	numDescriptors = 512;
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
			size_t largestAfterAlloc = pHeapManager.GetLargestFreeBlockSize();
			pHeapManager.GFree(pPtr);



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
		const size_t maxTestAllocationSize = sizeHeap / 2;


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

			bool success = pHeapManager.ContainsAddressInBlock(pPtr);
			assert(success);


			success = pHeapManager.IsAllocatedAddress(pPtr);
			assert(success);

			success = pHeapManager.GFree(pPtr);
			assert(success);
		}

		pHeapManager.GGCollect();
		// do a large test allocation to see if garbage collection worked
		size_t largest_block = pHeapManager.GetLargestFreeBlockSize();
		void * pPtr = pHeapManager.GAlloc(largest_block);
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

	const size_t 		sizeHeap = 1024*1024;
	const unsigned int 	numDescriptors = 512;
	const uint8_t		initial_alignment = 4;
	// Allocate memory for my test heap.
	void * pHeapMemory = _aligned_malloc(sizeHeap, 4);
	assert(pHeapMemory);

	// Create a heap manager for my test heap.
	GAllocator pHeapManager = GAllocator(DEFAULT_BLOCK_ALLOCATOR_SIZE, DEFAULT_NUM_OF_DESCRIPTORS, DEFAULT_BLOCK_ALLOCATOR_ALIGNMENT);


#ifdef TEST_SINGLE_LARGE_ALLOCATION
	// This is a test I wrote to check to see if using the whole block if it was almost consumed by 
	// an allocation worked. Also helped test my ShowFreeBlocks() and ShowOutstandingAllocations().
	{
		//ShowFreeBlocks(pHeapManager);

		size_t largestBeforeAlloc = pHeapManager.GetLargestFreeBlockSize();
		//void * pPtr = alloc(pHeapManager, largestBeforeAlloc - HeapManager::s_MinumumToLeave);
		void * pPtr = pHeapManager.GAlloc(largestBeforeAlloc);

		if (pPtr)
		{
#ifdef __TRACK_ALLOCATIONS
			ShowOutstandingAllocations(pHeapManager);
#endif // __TRACK_ALLOCATIONS

			size_t largestAfterAlloc = pHeapManager.GetLargestFreeBlockSize();
			pHeapManager.GFree(pPtr);

#ifdef __TRACK_ALLOCATIONS
			ShowOutstandingAllocations(pHeapManager);
#endif // __TRACK_ALLOCATIONS

			pHeapManager.GGCollect();

#ifdef __TRACK_ALLOCATIONS
			ShowOutstandingAllocations(pHeapManager);
#endif // __TRACK_ALLOCATIONS
			printf("\n");

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
		const size_t		maxTestAllocationSize = 1024;

		const unsigned int	alignments[] = { 4, 8, 16, 32, 64 };

		unsigned int	index = rand() % (sizeof(alignments) / sizeof(alignments[0]));

		unsigned int	alignment = alignments[index];

		size_t			sizeAlloc = 1 + (rand() & (maxTestAllocationSize - 1));

		//void * pPtr = alloc(pHeapManager, sizeAlloc, alignment);
		void * pPtr = pHeapManager.GAlloc(sizeAlloc, alignment);
		
		if (pPtr != NULL) {
			assert((reinterpret_cast<uintptr_t>(pPtr) & (alignment - 1)) == 0);
		}
		else {
			pHeapManager.GGCollect();

			pPtr = pHeapManager.GAlloc(sizeAlloc, alignment);

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
			
			bool success = pHeapManager.ContainsAddressInBlock(pPtr);
			assert(success);

			success = pHeapManager.IsAllocatedAddress(pPtr);
			assert(success);

			success = pHeapManager.GFree(pPtr);
			assert(success);
		}

#ifdef __TRACK_ALLOCATIONS
		ShowOutstandingAllocations(pHeapManager);
#endif // __TRACK_ALLOCATIONS

		// do garbage collection
		pHeapManager.GGCollect();

		//TODO: GARBAGE COLLECTING NEEDS TO ADD TO THE FREE LIST FROM THE FRONT
		//TODO: USE CASE OF IF USED ALL THE FREE NODES
		// our heap should be one single block, all the memory it started with


#ifdef __TRACK_ALLOCATIONS
		ShowOutstandingAllocations(pHeapManager);
#endif // __TRACK_ALLOCATIONS

		void * pPtr = pHeapManager.GAlloc(sizeHeap / 2);
		assert(pPtr);

		if (pPtr)
		{
			bool success = pHeapManager.GFree(pPtr);
			assert(success);

		}
	}

	_aligned_free(pHeapMemory);

	// we succeeded
	return true;
}

bool MonsterTesting::BitArrayTests() {

	for (size_t bit_count = 1; bit_count <= 1000; bit_count++) {
		GAllocator* my_allocator = GAllocator::GetInstance();
		BitArray* my_array = BitArray::Create(bit_count, my_allocator);

		size_t random_bit_to_set = rand() % bit_count;
		my_array->SetBit(random_bit_to_set);

		size_t firstSetBit = 0;
		size_t firstClearBit = 0;

		bool foundSetBit = my_array->GetFirstSetBit(firstSetBit);
		assert(foundSetBit && (firstSetBit == random_bit_to_set));

		my_array->ClearBit(random_bit_to_set);
		foundSetBit = my_array->GetFirstSetBit(firstSetBit);
		assert(foundSetBit == false);

		for (unsigned int i = 0; i < bit_count; i++)
		{
			assert(my_array->IsClear(i) == true);
			assert(my_array->IsSet(i) == false);

			size_t bit = 0;

			my_array->GetFirstClearBit(bit);
			assert(bit == i);

			my_array->SetBit(i);

			assert(my_array->IsClear(i) == false);
			assert(my_array->IsSet(i) == true);

			bool success = my_array->GetFirstClearBit(bit);
			assert(((i < (bit_count - 1)) && success && (bit == (i + 1))) || ((i == (bit_count - 1)) && !success));
		}

		my_array->SetAll();
		assert(my_array->GetFirstClearBit(firstClearBit) == false);

		my_array->ClearAll();
		assert(my_array->GetFirstSetBit(firstSetBit) == false);

		my_array->~BitArray();
	}

	return true;
}

bool MonsterTesting::FSATests() {

	GAllocator* my_allocator = GAllocator::GetInstance();
	for (size_t number_of_blocks = 1; number_of_blocks < 1000; number_of_blocks++) {

		const size_t size_of_blocks = 128;
		FixedSizeAllocator* my_fsa = FixedSizeAllocator::Create(my_allocator, number_of_blocks, size_of_blocks, my_allocator);
		BitArray* my_ba = my_fsa->GetArray();


		std::vector<void *> AllocatedAddresses;

		long	numAllocs = 0;
		long	numFrees = 0;
		long	numCollects = 0;

		bool	done = false;

		do {

			size_t	sizeAlloc = 1 + (rand() & (size_of_blocks - 1));
			numAllocs++;
			void * ptr = my_fsa->Falloc(sizeAlloc);

			if (ptr == NULL) {
				assert(my_ba->AreAllSet());
				break;
			}

			AllocatedAddresses.push_back(ptr);

		} while (1);


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
				bool success = my_fsa->Free(pPtr);

				assert(success);
			}

			assert(my_ba->AreAllClear());
		}

		my_fsa->~FixedSizeAllocator();
	}

	GAllocator::DestroyInstance();
	return true;

}

bool MonsterTesting::FinalHeapManagerTesting() {

	const size_t 		sizeHeap = 1024 * 1024;
	const unsigned int 	numDescriptors = 2048;

	// Allocate memory for my test heap.
	void * pHeapMemory = _aligned_malloc(sizeHeap, 4);
	assert(pHeapMemory);

	// Create a heap manager for my test heap.
	//MemoryManager * pHeapManager = MemoryManager(pHeapMemory, sizeHeap, numDescriptors);
	MemoryManager* pHeapManager = MemoryManager::GetInstance();
	assert(pHeapManager);

	if (pHeapManager == NULL)
		return false;

#ifdef TEST_SINGLE_LARGE_ALLOCATION
	// This is a test I wrote to check to see if using the whole block if it was almost consumed by 
	// an allocation worked. Also helped test my ShowFreeBlocks() and ShowOutstandingAllocations().
	{
		ShowFreeBlocks(pHeapManager);

		size_t largestBeforeAlloc = GetLargestFreeBlock(pHeapManager);
		void * pPtr = alloc(pHeapManager, largestBeforeAlloc - HeapManager::s_MinumumToLeave);

		if (pPtr)
		{
#ifdef __SHOW_FREE_BLOCKS
			ShowFreeBlocks(pHeapManager);
			printf("\n");
#endif // __SHOW_FREE_BLOCKS
#ifdef __SHOW_ALLOCATIONS
			ShowOutstandingAllocations(pHeapManager);
			printf("\n");
#endif // __SHOW_ALLOCATIONS

			size_t largestAfterAlloc = GetLargestFreeBlock(pHeapManager);
			free(pHeapManager, pPtr);

#ifdef __SHOW_FREE_BLOCKS
			ShowFreeBlocks(pHeapManager);
#endif // __SHOW_FREE_BLOCKS
#ifdef __SHOW_ALLOCATIONS
			ShowOutstandingAllocations(pHeapManager);
#endif // __SHOW_ALLOCATIONS
			printf("\n");

			Collect(pHeapManager);
#ifdef __SHOW_FREE_BLOCKS
			ShowFreeBlocks(pHeapManager);
#endif // __SHOW_FREE_BLOCKS
#ifdef __SHOW_ALLOCATIONS
			ShowOutstandingAllocations(pHeapManager);
#endif // __SHOW_ALLOCATIONS
			printf("\n");

			size_t largestAfterCollect = GetLargestFreeBlock(pHeapManager);
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

		size_t			sizeAlloc = 1 + (rand() & (maxTestAllocationSize - 1));

#ifdef SUPPORT_ALIGNMENT
		const unsigned int	alignments[] = { 4, 8, 16, 32, 64 };

		unsigned int	index = rand() % (sizeof(alignments) / sizeof(alignments[0]));

		unsigned int	alignment = alignments[index];

		void * pPtr = alloc(pHeapManager, sizeAlloc, alignment);

		assert((reinterpret_cast<uintptr_t>(pPtr) & (alignment - 1)) == 0);
#else
		void * pPtr = pHeapManager->Malloc(sizeAlloc);
#endif // SUPPORT_ALIGNMENT

		if (pPtr == NULL)
		{
			pHeapManager->GarbageCollectBlockAllocator();

#ifdef SUPPORT_ALIGNMENT
			pPtr = alloc(pHeapManager, sizeAlloc, alignment);
#else
			pPtr = pHeapManager->Malloc(sizeAlloc);
#endif // SUPPORT_ALIGNMENT

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

			bool success = pHeapManager->Free(pPtr);
			assert(success);

			numFrees++;
		}

		if ((rand() % garbageCollectAboutEvery) == 0)
		{
			pHeapManager->GarbageCollectBlockAllocator();

			numCollects++;
		}

	} while (1);

#ifdef __SHOW_FREE_BLOCKS
	printf("After exhausting allocations:\n");
	ShowFreeBlocks(pHeapManager);
#endif // __SHOW_FREE_BLOCKS

#ifdef __SHOW_ALLOCATIONS
	ShowOutstandingAllocations(pHeapManager);
#endif // __SHOW_ALLOCATIONS
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

			//bool success = Contains(pHeapManager, pPtr);
			//assert(success);

			//success = IsAllocated(pHeapManager, pPtr);
			//assert(success);

			bool success = pHeapManager->Free(pPtr);
			assert(success);
		}

#ifdef __SHOW_FREE_BLOCKS
		printf("After freeing allocations:\n");
		ShowFreeBlocks(pHeapManager);
#endif // __SHOW_FREE_BLOCKS

#ifdef __SHOW_ALLOCATIONS
		ShowOutstandingAllocations(pHeapManager);
#endif // __SHOW_ALLOCATIONS

		// do garbage collection
		pHeapManager->GarbageCollectBlockAllocator();
		// our heap should be one single block, all the memory it started with

#ifdef __SHOW_FREE_BLOCKS
		printf("After garbage collection:\n");
		ShowFreeBlocks(pHeapManager);
#endif // __SHOW_FREE_BLOCKS

#ifdef __SHOW_ALLOCATIONS
		ShowOutstandingAllocations(pHeapManager);
#endif // __SHOW_ALLOCATIONS

		printf("\n");		// do a large test allocation to see if garbage collection worked
		void * pPtr = pHeapManager->Malloc(sizeHeap / 2);
		assert(pPtr);

		if (pPtr)
		{
			bool success = pHeapManager->Free(pPtr);
			assert(success);

		}
	}

	pHeapManager->~MemoryManager();
	pHeapManager = NULL;

	_aligned_free(pHeapMemory);

	// we succeeded
	return true;
}