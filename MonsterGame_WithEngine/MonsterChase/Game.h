#pragma once


#include "Monster.h"
#include "MonsterEngine.h"
#include "MonsterPoint2D.h"
#include "Player.h"

#include <conio.h>
#include <iostream>
#include <vector>
class Game
{
public:
	Game(int board_size, MonsterPoint2D &player_start_pos, int kill_freq);
	~Game();
	void Start();

private:

	void AddMonster();
	void InitializeMonsters();
	void GetAndSetUserName();
	void GetAndSetNumberOfMonsters();

	//validity check(s)
	bool CheckForNumberValidity(const char * input) const;
	void PlayGame();

	//data display
	void DisplayGameState();

	Player * main_player_;

	void  KillMonster();
	

	int board_bounds_;
	int kill_monster_counter_;
	int kill_monsters_every_;
	int number_of_monsters_;
	int time_step_ = 1;
	

	std::vector<Monster*> master_monster_list;
	char user_name_buffer_[4096];
	char input_buffer_[4096];
};

#include "Game-inl.h"


