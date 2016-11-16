#include "MonsterAllocator.h"
#include "MonsterTesting.h"
#include "Game.h"
#include "MonsterGame.h"
#include "MonsterObject.h"
#include "Player.h"
//#define ALLOCATOR_TESTS
//#define CONST_TESTS
#define _GAMEPLAY
//#define MYTESTS

void RunAllocatorTests();
void RunMyAllocatorTests();
void RunConstTests();
void RunGame();
int main() {
	
	
#ifdef _GAMEPLAY
	RunGame();
	MonsterAllocator::DestroyInstance();
#endif // _GAMEPLAY

#ifdef CONST_TESTS
	RunConstTests();
#endif


#ifdef ALLOCATOR_TESTS
	RunAllocatorTests();
	RunMyAllocatorTests();
#endif // _ALLOCATORTESTS

#if defined _DEBUG
	_CrtDumpMemoryLeaks();
#endif // _DEBUG
	return 0;

}

void RunGame() {
	//new
	//MonsterPoint2D playerPos = MonsterPoint2D(30, 30);
	//Game mg = Game(100, playerPos, 3);
	//mg.Start();

	//old
	//MonsterPoint2D playerPos = MonsterPoint2D(30, 30);
	//MonsterGame mg = MonsterGame(100, playerPos);
	//mg.Start();

	Player *me = new Player("Hello");
	
}

void RunConstTests() {
	const MonsterPoint2D A(3.0, 3.0);
	const MonsterPoint2D B(2.0, 2.0);
	double result = A.DotProduct(B);

	double mag = A.Magnitude();
	double x_coord = A.x();
	double y_coord = A.y();

	MonsterPoint2D C = A + B;
	//A = A + 3; //not possible because A is const and the compiler hates you.
	//A = A - 3;
	
	MonsterPoint2D D = A - B;

}

void RunAllocatorTests() {
	bool result = MonsterTesting::RunAllocatorTests();
	assert(result);
}

void RunMyAllocatorTests() {
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


	pHeapManager.MonsterFree(addr1);
	pHeapManager.MonsterFree(addr2);
	pHeapManager.MonsterFree(addr3);
	pHeapManager.MonsterFree(addr4);
	//double val = 0xBBBBBBBBBB;
	//*((double*)addr1) = val;
	//void * killGuardbandAddr = static_cast<char*>(addr1) - 1;
	//*static_cast<__int8*>(killGuardbandAddr) = 0xB;
	//pHeapManager.MonsterFree(addr1);
}


