#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#define BUFFER 4096
#define WORDS 15
char * MakeSentence(const char ** input);

void main(int i_argc, char ** i_argl) {

	const char * strings[] = {
		"This",
		"is",
		"a",
		"test",
		NULL
	};

	char inputBuffer[BUFFER];
	const char * words[WORDS];
	int currentWord = 0;

	bool gettingInput = true;

	while (gettingInput) {
		printf("Please enter a word [ENTER to finish]: ");
		fgets(inputBuffer, BUFFER, stdin);
		char c = inputBuffer[0];
		if (inputBuffer[0] == '\n') {
			break;
		}
		else {
			words[currentWord] = BUFFER;
		}
		
	}

	char * result = MakeSentence(strings);

	printf("The Sentence is: %s", result);

	free(result);

	_getch();
}

char * MakeSentence(const char ** input) {


	
	int currentCharIndex = 0;
	int amountOfWords = 0;
	int numberOfChars = 0;

	while (input[amountOfWords] != NULL) {
		while (input[amountOfWords][currentCharIndex] != '\0') {
			numberOfChars++;
			currentCharIndex++;

		}
		amountOfWords++;
		currentCharIndex = 0;

	}

	int spacesNeeded = amountOfWords-1; //amount of words - 1
	int totalNeeded = spacesNeeded + numberOfChars+2; //including period and null terminator

	char* ptr = (char*)malloc(totalNeeded);

	//create sentence
	amountOfWords = 0;
	currentCharIndex = 0;
	int place = 0;
	while (input[amountOfWords] != NULL) {
		while (input[amountOfWords][currentCharIndex] != '\0') {
			ptr[place] = input[amountOfWords][currentCharIndex];
			currentCharIndex++;
			place++;
		}
		amountOfWords++;
		if (input[amountOfWords] != NULL) {
			ptr[place] = ' ';
			place++;
		}
		
		currentCharIndex = 0;
		
	}

	//period and null terminator
	ptr[place] = '.';
	++place;
	ptr[place] = '\0';

	return ptr;

}
