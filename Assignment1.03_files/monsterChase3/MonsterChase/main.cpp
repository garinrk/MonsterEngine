////////////////////////////////////////////////////////////////////////////////////////////////////
// file:	main.cpp
//
// summary:	Main class for MonsterChase game.
// Garin Richards
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <ctype.h>
#include <conio.h>
#include "Monster.h"
#include "MonsterDebug.h"

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
int GetRandomNumberInBounds(int min, int max);
int RandomTrueOrFalse();

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

double playerPosX = 50;
double playerPosY = 50;
int timeStep = 1;

int killMonstersEvery = 3;
int killMonsterCounter = killMonstersEvery;
int numberOfMonsters;

Monster *masterMonsterList;


int main() {
	printf("%s", "=======================================\n");
	printf("%s", "==============MONSTER CHASE============\n");
	printf("%s", "=======================================\n");
	DEBUGLOG("%d %c %d", 123, '<', 456);
	GetUserName();
	GetNumberOfMonsters();
	InitializeMonsters(numberOfMonsters);
	PlayGame();

	if (quitGameFlag) {
		printf("%s", "=======================================\n");
		printf("%s", "==============EXITING GAME=============\n");
		printf("%s", "=======================================\n");
	}


	//free array
	delete[] masterMonsterList;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Check for number validity, specifically if 
/// 			the user inputted a number less than 100 </summary>
///
/// <remarks>	Garin, 9/4/2016. </remarks>
///
/// <param name="input">	[in,out] If non-null, the input. </param>
///
/// <returns>	true if it succeeds, false if it fails. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CheckForNumberValidity(char* input)
{
	size_t result = strlen(input);
	int userNumber = atoi(input);
	if (userNumber > 100 || result == 0 || userNumber == 0)
		return false;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Gets user name using printf a </summary>
///
/// <remarks>	Garin, 9/4/2016. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

void GetUserName() {

	while (askingForPlayerInfo) {
		printf("%s","\nWhat is your name [Max length of 80]: ");
		fgets(userNameInput, sizeof(userNameInput), stdin);

		if (userNameInput != 0) {
			askingForPlayerInfo = false;
		}
		else
			printf("%s", "Invalid");
	}

	 
	printf("%s%s\n", "Accepted!\n\tWelcome ", userNameInput);

	
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Gets number of monsters via scanf </summary>
///
/// <remarks>	Garin, 9/4/2016. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

void GetNumberOfMonsters()
{
	//while (askingForNumberOfMonsters) {

	//	printf("%s", "\nHow many monsters would you like to create [100 max]: ");
	//	scanf_s("%s", &numberInput, 255);

	//	if (CheckForNumberValidity(numberInput))
	//		askingForNumberOfMonsters = false;
	//	else
	//		printf("%s", "Invalid\n");


	//}

	//numberOfMonsters = atoi(numberInput);
	while (askingForNumberOfMonsters) {

		printf("\nHow many monsters would you like to create? [100 Max]: ");
		
		fgets(numberInput, sizeof(numberInput), stdin);

		size_t result = strlen(numberInput);
		int userNumber = atoi(numberInput);
		if (userNumber > 100 || result == 0 || userNumber == 0) {
			printf("%s", "Invalid\n");
		}
		else {
			numberOfMonsters = userNumber;
			askingForNumberOfMonsters = false;
		}

	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Main gameplay function. </summary>
///
/// <remarks>	Garin, 9/4/2016. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

void PlayGame()
{
	printf("=======================================\n");
	printf("==============GAME START===============\n");
	printf("=======================================\n\n\n");
	while (inMainGameplayLoop) {
		
		DisplayGameState();

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

		//kill a monster randomly.
		killMonsterCounter--;
		if (killMonsterCounter == 0) {
			killMonsterCounter = killMonstersEvery;
			int monsterToKill = GetRandomNumberInBounds(0, numberOfMonsters - 1);
			KillMonster(monsterToKill);

		}

		
		for (int i = 0; i < numberOfMonsters; i++) {
			masterMonsterList[i].Update();
		}
		
		//add a monster randomly
		if (RandomTrueOrFalse())
			AddMonster();

		
		timeStep += 1;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Gets player input for the game. Acceptable input: WASD and Q </summary>
///
/// <remarks>	Garin, 9/4/2016. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

void GetPlayerInput() {

	bool processingPlayerInput = true;
	bool valid = true;

	while (processingPlayerInput) {
		
		printf("%s", "Move in the dungeon with WASD [q for quit]: ");
		char input = _getch();

		if (input != 'w' && input != 'a' && input != 's' && input != 'd' && input != 'q')
		{
			printf("%s", "\nINVALID INPUT [wasd or q for quit]\n");
			valid = false;
			continue;
		}

		else {
			ProcessPlayerInput(input);
			processingPlayerInput = false;
		}
	}

	
}

void DisplayGameState()
{

 	printf("\n\n=======================================\n");
	printf("==============Day Number %d=============\n",timeStep);
	printf("=======================================\n");
	printf("\n\n%s%d%s\n", "There are ",numberOfMonsters," monster(s) in the dungeon");

	for (int i = 0; i < numberOfMonsters; i++) {

		//Monster temp = masterMonsterList[i];
		printf("Monster %d is at %.1f, %.1f and is %d day(s) old\n", masterMonsterList[i].GetName(), masterMonsterList[i].pos.x(), masterMonsterList[i].pos.y(), masterMonsterList[i].age);
	}

	printf("You are at (%.1f, %.1f)\n\n", playerPosX, playerPosY);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Process the player input described by n. WASD moves the player
/// 			in the appropriate direction on the gameboard </summary>
///
/// <remarks>	Garin, 9/4/2016. </remarks>
///
/// <param name="n">	The char to process. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

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


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Initializes the monsters based on the amount of monsters
/// 			specified </summary>
///
/// <remarks>	Garin, 9/4/2016. </remarks>
///
/// <param name="n">	The int to process. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void InitializeMonsters(int n)
{
	masterMonsterList = new Monster[n];

	for (int i = 0; i < n; i++) {
		Monster tempMon;
		tempMon.SetName(i+1);
		tempMon.SetBoardBounds(boardSizeX, boardSizeY);
		tempMon.SetRandomPos();
		masterMonsterList[i] = tempMon;
		//printf("%s%d\n", "Creating monster ", i+1);

	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Kills a monster based on its given index in the 
/// 			master monster list. Does this by copying over it and 
/// 			shifting the array over. </summary>
///
/// <remarks>	Garin, 9/4/2016. </remarks>
///
/// <param name="monsterPos">	The monster position. </param>
////////////////////////////////////////////////////////////////////////////////////////////////////

void KillMonster(int monsterPos)
{
	printf("\n\n=======================================\n");
	printf("===========A MONSTER HAS DIED==========\n");
	printf("=======================================\n\n");
	numberOfMonsters--;
	for (int i = monsterPos; i < numberOfMonsters-1; i++) {
		masterMonsterList[i] = masterMonsterList[i + 1];
		masterMonsterList[i].SetName(i+1);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Gets random number in bounds. A helper function </summary>
///
/// <remarks>	Garin, 9/4/2016. </remarks>
///
/// <param name="min">	The minimum. </param>
/// <param name="max">	The maximum. </param>
///
/// <returns>	The random number in bounds. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

int GetRandomNumberInBounds(int min, int max) {

	srand((unsigned int)time(NULL));
	return min + rand() % (max - min + 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Adds a new monster to the master monster list. </summary>
///
/// <remarks>	Garin, 9/4/2016. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

void AddMonster() {

	numberOfMonsters++;
	printf("\n\n=======================================\n");
	printf("===========A MONSTER HAS APPEARED======\n");
	printf("=======================================\n\n");

	//create new array
	Monster newMon;
	newMon.SetName(numberOfMonsters);
	newMon.SetBoardBounds(boardSizeX, boardSizeY);
	newMon.SetRandomPos();
	
	Monster *newArray = new Monster[numberOfMonsters];

	for (int i = 0; i < numberOfMonsters; i++) {

		if (i < numberOfMonsters - 1) {
			newArray[i] = masterMonsterList[i];
		}
		if (i == numberOfMonsters-1)
			newArray[i] = newMon;
		
	}

	masterMonsterList = newArray;
}

int RandomTrueOrFalse() {

	srand((unsigned int)time(NULL));
	return rand() % 2;
}


