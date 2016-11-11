#ifndef MONSTERGAME_H
#define MONSTERGAME_H

#include "MonsterPoint2D.h"
#include "Monster.h"
#include "MonsterAllocator.h"
#include "MonsterDebug.h"
#include "MonsterEngine.h"
#include "Player.h"
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

class MonsterGame
{
public:
	MonsterGame(int boardSize, MonsterPoint2D &playerStartPos);
	~MonsterGame();
	void Start();


private:

	bool CheckForNumberValidity(char* input) const;
	void InitializeMonsters(const int numberOfMonsters);
	void GetAndDisplayUserName();
	void GetAndSetNumberOfMonsters();
	void PlayGame();

	void DisplayGameState() const;
	void KillMonster(const int monsterPos);
	void AddMonster();


	bool askingForPlayerInfo = true;
	bool askingForNumberOfMonsters = true;
	bool inMainGameplayLoop = true;
	
	bool killOrSpawnFlag = false;

	bool validCheck;
	char userNameInput[4096];
	char numberInput[4096];

	int boardSizeX = 100;
	int boardSizeY = 100;

	Player  *main_player;
	int timeStep = 1;

	int killMonstersEvery = 3;
	int killMonsterCounter = killMonstersEvery;
	int numberOfMonsters;

	Monster *masterMonsterList;

};
#include "MonsterGame-inl.h"
#endif