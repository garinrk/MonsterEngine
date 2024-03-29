#ifndef MONSTERGAME_H
#define MONSTERGAME_H

#include "Point2D.h"
#include "Monster.h"
#include "MonsterAllocator.h"
#include "MonsterDebug.h"
#include "MonsterEngine.h"
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

class MonsterGame
{
public:
	MonsterGame(int boardSize, Point2D playerStartPos);
	~MonsterGame();
	void Start();


private:

	bool CheckForNumberValidity(char* input);
	void InitializeMonsters(int numberOfMonsters);
	void GetUserName();
	void GetNumberOfMonsters();
	void PlayGame();
	void GetPlayerInput();
	void DisplayGameState();
	void ProcessPlayerInput(char n);
	void KillMonster(int monsterPos);
	void AddMonster();


	bool askingForPlayerInfo = true;
	bool askingForNumberOfMonsters = true;
	bool inMainGameplayLoop = true;
	bool quitGameFlag = false;
	bool killOrSpawnFlag = false;

	bool validCheck;
	char userNameInput[4096];
	char numberInput[4096];

	int boardSizeX = 100;
	int boardSizeY = 100;

	double playerPosX;
	double playerPosY;
	int timeStep = 1;

	int killMonstersEvery = 3;
	int killMonsterCounter = killMonstersEvery;
	int numberOfMonsters;

	Monster *masterMonsterList;

};

#endif