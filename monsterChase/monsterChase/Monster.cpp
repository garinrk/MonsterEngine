//Monster.cpp - Garin Richards
#include "Monster.h"
#include <iostream>

Monster::Monster() {
	time(&rawTime);

	localtime_s(&now,&rawTime);

	timeSinceNewYear.tm_hour = 0;
	timeSinceNewYear.tm_min = 0;
	timeSinceNewYear.tm_sec = 0;
	timeSinceNewYear.tm_mon = 0;
	timeSinceNewYear.tm_mday = 1;

	name = difftime(mktime(&now), mktime(&timeSinceNewYear));

	printf("The monster %.f has been created.\n",name);
}

void Monster::MoveMonster() {

}

void Monster::CheckForDeath() {

}