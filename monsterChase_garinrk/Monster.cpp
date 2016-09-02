#include <iostream>
#include <string>

class Monster {

public: 
	void SetInitalPositions(int boardX, int boardY);
	void MoveMonster(int xdir, int ydir);
	bool CheckForDeath();

private:
	int xPos;
	int yPos;
	int health;
};

void Monster::SetInitalPositions(int boardX, int boardY) {
	
}