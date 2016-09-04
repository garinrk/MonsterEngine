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



class Monster {

	public:
		Monster();
		void MoveMonster();
		bool CheckForDeath();
		void SetName(int n);
		void SetPos();
		void SetBoardBounds(int x, int y);
		void Update();
		int GetName();
		int xPos;
		int yPos;
		int age = 1;

	private:
		void MakeOlder(); 
		int boundX;
		int boundY;
		int name;
		int maxAge = 30;
};


#endif