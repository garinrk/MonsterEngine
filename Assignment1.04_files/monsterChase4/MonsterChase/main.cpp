#include "MonsterAllocator.h"
#include "MonsterTesting.h"
#include "MonsterGame.h"


void AllocatorTesting();

int main() {

	Point2D playerPos = Point2D(30, 30);
	MonsterGame mg = MonsterGame(100,  playerPos);
	mg.Start();

	//AllocatorTesting();

	return 0;
}

void AllocatorTesting() {


	MonsterTesting mt;
	mt.RunAllocatorTests();
	


}


