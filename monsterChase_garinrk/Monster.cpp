#include <iostream>
#include <string>
#include <ctime>

class Monster {

public:
	void MoveMonster(int xdir, int ydir);
	bool CheckForDeath();
	Monster::Monster() {

	}

private:
	int xPos;
	int yPos;
	int health;
	int age;
	int name;

};

	void Monster::MoveMonster(int xdir, int ydir)
	{
	}

	bool Monster::CheckForDeath()
	{
		return false;
	}
