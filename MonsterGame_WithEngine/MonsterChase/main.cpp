#include "MonsterAllocator.h"
#include "GAllocator.h"
#include "MonsterTesting.h"
#include "Game.h"
#include "MonsterGame.h"
#include "MonsterObject.h"
#include "Player.h"
#include "MMath.h"
#include "BitArray.h"
//#define NEW_ALLOCATOR_TESTS
//#define CONST_TESTS
//#define OLD_GAMEPLAY
//#define NEW_GAMEPLAY
#define BIT_ARRAY_TESTS
//#define NEW_TESTS
//#define NAN_TEST

void RunConstTests();
void RunGame();
void RunNewGame();
void NANTests();
void RunNewTests();
void RunGAllocTests();
void ArrayTests();

int main() {
	
	
#ifdef OLD_GAMEPLAY
	RunGame();
#endif // _GAMEPLAY

#ifdef NEW_GAMEPLAY
	GAllocator::CreateInstance(TOTAL_SIZE, NUM_DESCRIPTORS, ALIGNMENT);
	RunNewGame();
	GAllocator::DestroyInstance();
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

#ifdef NEW_ALLOCATOR_TESTS
	RunGAllocTests();
#endif // NEW_ALLOCATOR_TESTS

#ifdef BIT_ARRAY_TESTS
	ArrayTests();
#endif


	

#if defined _DEBUG
	_CrtDumpMemoryLeaks();
#endif // _DEBUG
	return 0;

}

void RunNewGame() {
	MonsterPoint2D playerPos = MonsterPoint2D(30, 30);
	Game mg = Game(100, playerPos, 3);
	mg.Start();

}

void RunGame() {

	MonsterPoint2D playerPos = MonsterPoint2D(30, 30);
	MonsterGame mg = MonsterGame(100, playerPos);
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
	MonsterTesting::GAllocatorTests();
	MonsterTesting::GAllocatorWithAlignmentTests();
}

void ArrayTests()
{
	size_t numberOfBits = 16;
	//BitArray myArray = BitArray(numberOfBits);
	size_t bitToClear = 3;
	GAllocator* my_allocator = GAllocator::GetInstance();
	BitArray* my_array = BitArray::Create(16, false, my_allocator);

	//my_array->ClearBit(bitToClear);
	//assert(my_array->IsClear(bitToClear));

	//my_array->SetBit(bitToClear);
	//assert(my_array->IsSet(bitToClear));
	for (size_t i = 0; i < 16; i++) {
		my_array->ClearBit(i);
	}

	my_array->ClearAll();
	assert(my_array->AreAllClear());
	my_array->SetAll();
	assert(my_array->AreAllSet());
	
	my_array->~BitArray();

}

