#include <iostream>
#include <string>
using namespace std;

bool askingForInfo = true;

string GetConsoleInput();

string userInput;
bool validCheck;
bool CheckForValidity(string input);
int maxUserInputLength = 255;


int main() {


	while (askingForInfo){ 
		std::cout << "Enter stuff: ";
		userInput = GetConsoleInput();
		validCheck = CheckForValidity(userInput);
		cout << "Input: " << userInput << " Valid: " << validCheck << endl;
	}

	return 0;
}



string GetConsoleInput()
{
	string r;

	getline(cin, r);

	return r;
}

bool CheckForValidity(string input) {


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