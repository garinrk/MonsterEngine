////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	Monster.h
//
// summary:	Declares the monster class
// Garin Richards
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MONSTER_H
#define MONSTER_H

#include <random>
#include <time.h>
#include "Point2D.h"



class Monster {

	public:
		Monster();
		void MoveMonster();
		bool CheckForDeath();
		void SetName(int n);
		void SetRandomPos();
		void SetBoardBounds(int x, int y);
		void Update();
		int GetName();
		Point2D pos = Point2D(0, 0);

		int age = 1;

	private:
		void MakeOlder(); 
		int boundX;
		int boundY;
		int name;
		int maxAge = 30;
};


#endif