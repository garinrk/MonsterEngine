#include "MonsterAllocator.h"
#include "MonsterTesting.h"
#include "MonsterGame.h"

#define _TESTING
//#define _GAMEPLAY


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
	return 0;
}



