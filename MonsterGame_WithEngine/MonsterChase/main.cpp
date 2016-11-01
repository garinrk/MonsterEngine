#include "MonsterAllocator.h"
#include "MonsterTesting.h"
#include "MonsterGame.h"

#define ALLOCATORTESTS
//#define _GAMEPLAY
#define _TESTING

int main() {

#ifdef _GAMEPLAY
	Point2D playerPos = Point2D(30, 30);
	MonsterGame mg = MonsterGame(100, playerPos);
	mg.Start();
#endif // _GAMEPLAY


#ifdef ALLOCATORTESTS
	bool result = MonsterTesting::RunAllocatorTests();
	assert(result);
#endif // _ALLOCATORTESTS

#ifdef _TESTING
	const size_t 		sizeHeap = 1024;
	const unsigned int 	numDescriptors = 8;
	const size_t		align = 4;
	 
	// Create a heap manager for my test heap.
	MonsterAllocator pHeapManager = MonsterAllocator(sizeHeap, numDescriptors);
	void* addr1 = pHeapManager.MonsterMalloc(1);
	void* addr2 = pHeapManager.MonsterMalloc(1);
	void* addr3 = pHeapManager.MonsterMalloc(1);
	void* addr4 = pHeapManager.MonsterMalloc(1);
	*((__int8*)addr1) = 0x11;
	*((__int8*)addr2) = 0x22;
	*((__int8*)addr3) = 0x33;
	*((__int8*)addr4) = 0x44;


	pHeapManager.MonsterFree(addr2);
	pHeapManager.MonsterFree(addr3);
	pHeapManager.MonsterFree(addr1);
	pHeapManager.MonsterFree(addr4);
	
	//double val = 0xBBBBBBBBBB;
	//*((double*)addr1) = val;
	//void * killGuardbandAddr = static_cast<char*>(addr1) - 1;
	//*static_cast<__int8*>(killGuardbandAddr) = 0xB;
	//pHeapManager.MonsterFree(addr1);
#endif // _TESTING
	return 0;
}



