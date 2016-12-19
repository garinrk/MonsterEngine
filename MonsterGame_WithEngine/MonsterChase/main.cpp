#include "GAllocator.h"
#include "MonsterTesting.h"
#include "Game.h"
#include "Player.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//#define CONST_TESTS
#define GAMEPLAY
//#define NEW_TESTS
//#define NAN_TEST

//#define BLOCK_ALLOCATOR_TESTS
//#define BIT_ARRAY_TESTS
//#define FSA_TESTS
//#define FINAL_MEMORY_TESTING

void RunConstTests();
void RunNewGame();
void NANTests();
void RunNewTests();
void RunGAllocTests();
void ArrayTests();
void RunFSATest();
void RunMMTests();

int main() {

	//memory leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//GAllocator::CreateInstance();


#ifdef GAMEPLAY
	RunNewGame();
	MemoryManager::DestroyInstance();
#endif

#ifdef CONST_TESTS
	RunConstTests();
#endif


#ifdef NEW_TESTS
	RunNewTests();
#endif // NEW_TESTS

#ifdef NAN_TEST
	NANTests();
#endif

#ifdef BLOCK_ALLOCATOR_TESTS
	RunGAllocTests();
#endif // BLOCK_ALLOCATOR_TESTS

#ifdef BIT_ARRAY_TESTS
	ArrayTests();
#endif

#ifdef FSA_TESTS
	RunFSATest();
#endif

#ifdef FINAL_MEMORY_TESTING
	RunMMTests();
#endif // MM_TESTS
	
	return 0;

}

void RunNewGame() {
	MonsterPoint2D playerPos = MonsterPoint2D(30, 30);
	Game mg = Game(100, playerPos, 3);
	mg.Start();
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

void RunNewTests(){

	//overridden new uses my allocator
	Player *p1 = new Player("PlayerOne");
	delete p1;

	//support for supplementary debug information
	Player *p2 = new("Debug Information\n") Player("PlayerTwo");
	delete p2;
	//destroy the singleton.
	GAllocator::DestroyInstance();
}

void NANTests() {

	//true
	//bool test = MMEngine::IsNAN(nanf("")); 
	bool test = MMEngine::MMath::IsNAN(nanf(""));

	assert(test);


	//false
	bool test2 = MMEngine::MMath::IsNAN(5.0f);
	//bool test2 = MMEngine::IsNAN(5.0f);

	assert(!test2);

	
	bool test3 = MMEngine::MMath::AreEqual(4.0f, 4.0f);
	assert(test3);

	bool test4 = MMEngine::MMath::AreEqual(4.0f, 4.000001f);
	assert(!test4);

}

void RunGAllocTests() {
	assert(MonsterTesting::GAllocatorTests());
	assert(MonsterTesting::GAllocatorWithAlignmentTests());
}

void ArrayTests()
{
	assert(MonsterTesting::BitArrayTests());

}

void RunFSATest() {
	
	assert(MonsterTesting::FSATests());
}

void RunMMTests() {
	assert(MonsterTesting::FinalHeapManagerTesting());
}


