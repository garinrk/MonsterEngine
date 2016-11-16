#pragma once


#include "Monster.h"
#include "MonsterPoint2D.h"
#include "Player.h"

#include <conio.h>
#include <iostream>
class Game
{
public:
	Game(int board_size, MonsterPoint2D &player_start_pos, int kill_freq);
	~Game();
	void Start();

private:

	void InitializeMonsters();
	void GetAndSetUserName();
	void GetAndSetNumberOfMonsters();

	//validity check(s)
	bool CheckForNumberValidity(const char * input) const;
	void PlayGame();

	//data display
	void DisplayGameState() const;

	Player * main_player_;
	int time_step_;

	

	int board_bounds_;
	int kill_monster_counter_;
	int number_of_monsters_;
	

	Monster* master_monster_list_;
	char user_name_buffer_[4096];
	char input_buffer_[4096];
};

#include "Game-inl.h"


