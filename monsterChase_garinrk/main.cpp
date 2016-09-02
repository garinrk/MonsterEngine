#include <iostream>
#include <string>
using namespace std;

//forward declarations
bool CheckForNameValidity(string input);
bool CheckForNumberValidity(string input);
void ExitGame();

bool askingForPlayerInfo = true;
bool askingForNumberOfMonsters = true;
bool inMainGameplayLoop = true;
bool quitGameFlag = false;
string GetConsoleInput();

string userInput;
bool validCheck;
int maxUserInputLength = 255;
string welcomeMessage1 = "=======================================";
string welcomeMessage2 = "==============MONSTER CHASE============";
string welcomeMessage3 = "================Q to quit=============";

int main() {

	cout << welcomeMessage1 << endl << welcomeMessage2 << endl << welcomeMessage3 << endl;

	while (askingForPlayerInfo){ 
		cout << "Please enter your name (Letters only): ";
		userInput = GetConsoleInput();
		validCheck = CheckForNameValidity(userInput);
		if (quitGameFlag)
			return 0;
		if (validCheck == 1) 
			askingForPlayerInfo = false;
	}

	while (askingForNumberOfMonsters) {
		cout << "How many monsters should we start with?: ";
		userInput = GetConsoleInput();
		if (quitGameFlag)
			return 0;
		validCheck = CheckForNumberValidity(userInput);

		if (validCheck == 1)
			askingForNumberOfMonsters = false;
	}

	while (inMainGameplayLoop) {

	}

	return 0;
}



/// <summary>
/// Gets input from the console while also checking for
///a quit flag
/// </summary>
/// <returns>string from the console via getline</returns>
string GetConsoleInput()
{
	string r;

	getline(cin, r);

	if (r[0] == 'Q' || 'q')
		quitGameFlag = true;

	return r;
}



/// <summary>
/// Checks for name validity.
/// </summary>
/// <param name="input">string to check</param>
/// <returns>true if valid, false ow</returns>
bool CheckForNameValidity(string input) {


	if (static_cast<int>(input.size()) > maxUserInputLength) {
		return false;
	}

	//check for numbers
	for (unsigned int i = 0; i < input.length(); i++) {
		char c = input[i];

		if (isdigit(c))
			return false;
	}

	return true;
}


bool CheckForNumberValidity(string input) {

	if (static_cast<int>(input.size()) > maxUserInputLength) {
		return false;

		//check for digits
		for (unsigned int i = 0; i < input.length(); i++) {
			char c = input[i];

			if (!isdigit(c))
				return false;
		}

		return true;
	}
}

void ExitGame() {
	cout << "Goodbye!" << endl;
	
}