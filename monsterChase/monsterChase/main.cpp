////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	main.cpp
//
// summary:	Main class for MonsterChase game.
// Garin Richards
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string.h>
#include <ctype.h>
#include "Monster.h"


bool CheckForNumberValidity(char* input);
void InitializeMonsters(int numberOfMonsters);
void GetUserName();
void GetNumberOfMonsters();
void PlayGame();
void GetPlayerInput();
void DisplayGameState();
void ProcessPlayerInput(char n);
bool CheckForValidGameInput(char n);
void MaybeAddMonsters();
void KillMonster(int monsterPos);
void AddMonster();
int GetRandomNumberInBounds(int min, int max);
int RandomTrueOrFalse();

bool askingForPlayerInfo = true;
bool askingForNumberOfMonsters = true;
bool inMainGameplayLoop = true;
bool quitGameFlag = false;

bool validCheck;
char userNameInput[255];
char numberInput[3];
char gameplayInput[1];
int boardSizeX = 1000;
int boardSizeY = 1000;

int playerPosX = 0;
int playerPosY = 0;

int numberOfMonsters;
Monster *masterMonsterList;


int main() {
	printf("%s", "=======================================\n");
	printf("%s", "==============MONSTER CHASE============\n");
	printf("%s", "=======================================\n");

	GetUserName();
	GetNumberOfMonsters();
	//printf("\nCreating %d monsters\n", numberOfMonsters);
	InitializeMonsters(numberOfMonsters);
	PlayGame();

	if (quitGameFlag) {
		printf("%s", "=======================================\n");
		printf("%s", "==============EXITING GAME=============\n");
		printf("%s", "=======================================\n");
	}


	delete [] masterMonsterList;
	masterMonsterList = NULL;
	return 0;
}


bool CheckForNumberValidity(char* input)
{
	size_t result = strlen(input);
	int userNumber = atoi(input);
	if (userNumber > 100 || result == 0 || userNumber == 0)
		return false;
	return true;
}



void GetUserName() {

	while (askingForPlayerInfo) {
		printf("%s","\nWhat is your name [Max length of 80]: ");
		scanf_s("%s", &userNameInput, 80);

		if (userNameInput != 0)
			askingForPlayerInfo = false;
		else
			printf("%s", "Invalid");
	}

	printf("%s%s\n", "Accepted!\n\tWelcome ", userNameInput);
}
void GetNumberOfMonsters()
{
	while (askingForNumberOfMonsters) {

		printf("%s", "\nHow many monsters would you like to create [100 max]: ");
		scanf_s("%s", &numberInput, 255);

		if (CheckForNumberValidity(numberInput))
			askingForNumberOfMonsters = false;
		else
			printf("%s", "Invalid\n");


	}

	numberOfMonsters = atoi(numberInput);
}
void PlayGame()
{
	printf("GAME START\n");
	while (inMainGameplayLoop) {
		

		//get user input and process it
		GetPlayerInput();
		if (quitGameFlag)
			return;


		/*Main Loop :
		Query user for Player movement directory or quit request.
			If quit condition has been satisfied quit the game.Otherwise :
			Update Monster positions based on their AI.
			Determine if any Monsters need to be created or destroyed and do so.
			Update Player position based on user input.
			Go to step 1.
*/
		//UPDATE
		for (int i = 0; i < numberOfMonsters; i++) {
			int kill = masterMonsterList[i].CheckForDeath();
			if (kill == 1) {
				KillMonster(i);
				break;
			}
			else {
				masterMonsterList[i].Update();
			}
		}

		//maybe add or destroy monsters
		MaybeAddMonsters();
	}
}

void GetPlayerInput() {
	bool valid;
	printf("%s", "[wasd or q for quit]: ");
	scanf_s("%s", &gameplayInput, 1);

	valid = CheckForValidGameInput(gameplayInput[0]);

	if (quitGameFlag)
		return;

	if (valid == 1)
		ProcessPlayerInput(gameplayInput[0]);
	
}

void DisplayGameState()
{
}

void ProcessPlayerInput(char n) {
	n = tolower(n);
	if (n == 'w')
		playerPosY++;
	else if (n == 'a')
		playerPosX--;
	else if (n == 's')
		playerPosY--;
	else if (n == 'd')
		playerPosX++;
	else if (n == 'q')
		quitGameFlag = true;

}

bool CheckForValidGameInput(char n) {
	n = tolower(n);
	if (n == 'w' || n == 'a' || n == 's' || n == 'd' || n == 'q')
		return true;
	else {
		printf("%s", "\nInvalid Input ");
		return false;
	}
}


void InitializeMonsters(int n)
{
	masterMonsterList = new Monster[n];

	for (int i = 0; i < n; i++) {
		Monster tempMon;
		tempMon.SetName(i);
		tempMon.SetBoardBounds(boardSizeX, boardSizeY);
		tempMon.SetPos();
		masterMonsterList[i] = tempMon;
		printf("%s%d\n", "Creating monster ", i+1);

	}
}

void MaybeAddMonsters()
{
	int shouldAddMonster = RandomTrueOrFalse();
	if (shouldAddMonster == 1) {
		AddMonster();
	}
}

void KillMonster(int monsterPos)
{
	for (int i = monsterPos; i < numberOfMonsters; i++) {
		masterMonsterList[i] = masterMonsterList[i + 1];
	}
}

int GetRandomNumberInBounds(int min, int max) {

	srand((unsigned int)time(NULL));
	return min + rand() % (max - min + 1);
}

void AddMonster() {
	numberOfMonsters++;
	Monster newMon;
	newMon.SetName(999);
	newMon.SetBoardBounds(boardSizeX, boardSizeY);
	newMon.SetPos();

	Monster *newMonsterList = new Monster[numberOfMonsters];
	newMonsterList = masterMonsterList;
	newMonsterList[numberOfMonsters] = newMon;

}

int RandomTrueOrFalse() {

	srand((unsigned int)time(NULL));
	return rand() % 2;
}


