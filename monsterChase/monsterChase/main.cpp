#include <iostream>
#include <string.h>
#include <stdio.h>

bool CheckForNumberValidity(char* input);
void InitializeMonsters(int numberOfMonsters);
void CreateGame();
void PlayGame();
void ExitGame();

bool askingForPlayerInfo = true;
bool askingForNumberOfMonsters = true;
bool inMainGameplayLoop = true;
bool quitGameFlag = false;

bool validCheck;
int maxUserInputLength = 255;

char userInput[80];
char numberOfMonsters[3];


int main() {
	printf("%s", "=======================================\n");
	printf("%s", "==============MONSTER CHASE============\n");
	printf("%s", "=======================================\n");

	//TODO: User input error checking
	printf("%s", "What is your name: ");
	scanf_s("%s", &userInput,80);
	printf("\nWelcome %s\n", userInput);

	while (askingForNumberOfMonsters) {

		printf("%s", "\nHow many monsters would you like to create [100 max]: ");
		scanf_s("%s", &numberOfMonsters, 255);

		if (CheckForNumberValidity(numberOfMonsters))
			askingForNumberOfMonsters = false;
		
		printf("%s", "Invalid");
	}
	printf("Creating %s monsters", numberOfMonsters);


	
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

void InitializeMonsters(int numberOfMonsters)
{
}

void CreateGame()
{
}

void PlayGame()
{
}

void ExitGame()
{
}