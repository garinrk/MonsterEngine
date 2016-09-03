//Monster.h - Garin Richards
#ifndef MONSTER_H
#define MONSTER_H

#include <time.h>


class Monster {

	public:
		Monster();
		void MoveMonster();
		void CheckForDeath();
		

	private:
		int xPos;
		int yPos;
		int health;
		int age;
		double name;
		struct tm timeSinceNewYear;
		time_t rawTime;
		tm now;
};


#endif