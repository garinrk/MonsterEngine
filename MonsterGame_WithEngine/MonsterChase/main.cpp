#include "MonsterAllocator.h"
#include "MonsterTesting.h"
#include "MonsterGame.h"

//#define _TESTING
//#define _GAMEPLAY
#define _TESTING2

int main() {

#ifdef _GAMEPLAY
	Point2D playerPos = Point2D(30, 30);
	MonsterGame mg = MonsterGame(100, playerPos);
	mg.Start();
#endif // _GAMEPLAY


#ifdef _TESTING
	bool result = MonsterTesting::RunAllocatorTests();
	assert(result);
#endif // _TESTING

#ifdef _TESTING2
	const size_t 		sizeHeap = 1024;
	const unsigned int 	numDescriptors = 8;
	const size_t		align = 8;
	 
	// Create a heap manager for my test heap.
	MonsterAllocator pHeapManager = MonsterAllocator(sizeHeap, numDescriptors, align);
	void* addr = pHeapManager.MonsterMalloc(1);
	//*((int*)addr) = 0xBB;
	void * killGuardbandAddr = static_cast<char*>(addr) - 1;
	*static_cast<int*>(killGuardbandAddr) = 0xBB;
	pHeapManager.MonsterFree(addr);
#endif // _TESTING
	return 0;
}



