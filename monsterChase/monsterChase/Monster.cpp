////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	Monster.cpp
//
// summary:	Implements the monster class for MonsterChase game.
// Garin Richards
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Monster.h"


//constructor
Monster::Monster() {
	srand((unsigned int)time(NULL));

}

void Monster::SetName(int n) {
	name = n;
	//printf("Set name to %d\n", n);
}


void Monster::MoveMonster() {

	int moveOrNot = rand() % 2;

	if (moveOrNot == 1) {
		int moveX = rand() % (2);
		int moveY = rand() % (2);

		xPos += moveX;
		yPos += moveY;
	}
}

bool Monster::CheckForDeath() {
	int killOrNot = rand() % 2;

	if (killOrNot == 1) {
		return true;
	}
	
	return false;
}

void Monster::Update() {
	MoveMonster();
	MakeOlder();
}

void Monster::MakeOlder() {
	age++;
}

void Monster::SetPos() {
	xPos = rand() % (boundX + 1);
	yPos = rand() % (boundY+ 1);
}

void Monster::SetBoardBounds(int x, int y) {
	boundX = x;
	boundY = y;
}

int Monster::GetName() {
	return name;
}