#include <iostream>
#include <string.h>
#include "Monster.h"

bool CheckForNumberValidity(char* input);
void InitializeMonsters(int numberOfMonsters);
void GetUserName();
void GetNumberOfMonsters();
void PlayGame();

bool askingForPlayerInfo = true;
bool askingForNumberOfMonsters = true;
bool inMainGameplayLoop = true;
bool quitGameFlag = false;

bool validCheck;
char userNameInput[255];
char numberInput[3];
int boardSizeX = 100;
int boardSizeY = 100;
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
			printf("%s", "Invalid");


	}

	numberOfMonsters = atoi(numberInput);
}
void PlayGame()
{
	printf("Welcome to Monster Game!");
	while (inMainGameplayLoop) {
		
		//get user input

		//update monsters

		for (int i = 0; i < numberOfMonsters; i++) {
			masterMonsterList[i].Update();
		}
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

