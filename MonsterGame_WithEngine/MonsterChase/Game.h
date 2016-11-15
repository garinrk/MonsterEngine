#ifndef GAME_H
#define GAME_H

#include "MonsterPoint2D.h"

class Game
{
public:
	Game(int board_size, MonsterPoint2D &player_start_pos);
	~Game();
	void StartGame();

private:
	int board_size_;
	MonsterPoint2D player_start_;
};

#include "Game-inl.h"

#endif // !GAME_H

