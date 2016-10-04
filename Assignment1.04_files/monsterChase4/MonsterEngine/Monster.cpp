////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	Monster.cpp
//
// summary:	Implements the monster class for MonsterChase game.
// Garin Richards
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Monster.h"



//constructor
Monster::Monster() {
	 
}

void Monster::SetName(int n) {
	name = n;
}


void Monster::MoveMonster() {
	
	int moveOrNot = rand() % 2;

	if (moveOrNot == 1) {
		int moveX = rand() % (2);
		int moveY = rand() % (2);

		Point2D posChange;
		posChange.x(moveX);
		posChange.y(moveY);
		pos = pos + posChange;

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

void Monster::SetRandomPos() {
	double x = rand() % (boundX + 1);
	double y = rand() % (boundY+ 1);
	pos.x(x);
	pos.y(y);
}

void Monster::SetBoardBounds(int x, int y) {
	boundX = x;
	boundY = y;
}

int Monster::GetName() {
	return name;
}