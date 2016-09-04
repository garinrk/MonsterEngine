//Monster.cpp - Garin Richards
#include "Monster.h"
#include <iostream>

//constructor
Monster::Monster() {
	srand(time(NULL));

}

void Monster::SetName(int n) {
	name = n;
	//printf("Set name to %d\n", n);
}

void Monster::MoveMonster() {

}

void Monster::CheckForDeath() {
	
}

void Monster::Update() {
	MoveMonster();
	CheckForDeath();
	MakeOlder();
}

void Monster::MakeOlder() {
	age++;
}

void Monster::SetPos() {
	xPos = rand() % (boundX + 1);
	yPos = rand() % (boundY+ 1);
	//min + rand() % (max - min+1);
}

void Monster::SetBoardBounds(int x, int y) {
	boundX = x;
	boundY = y;
}