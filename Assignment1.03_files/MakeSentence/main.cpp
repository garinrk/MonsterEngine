#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#else
#include <stdlib.h>
#endif // _DEBUG

#define BUFFER 4096
#define WORDS 11
char * MakeSentence(const char * input[]);
char * Sanitize(char* in);
char * GetUserInput();

void main(int i_argc, char ** i_argl) {

	//const char * strings[] = {
	//	"This",
	//	"is",
	//	"a",
	//	"test",
	//	NULL
	//};


	//bool gettingInput = true;
	//while (wordLimit > 0 && gettingInput == true) {
	//	char* inputBuffer = (char*)malloc(256 * sizeof(char));
	//	printf("Please enter 10 words (%d left): ", wordLimit);
	//	fgets(inputBuffer, 256, stdin);

	//	if (inputBuffer[0] == '\n')
	//		continue;

	//	inputBuffer = Sanitize(inputBuffer);

	//	words[currentWord] = inputBuffer;
	//	currentWord++;
	//	wordLimit--;


	//}

/*
	char * inputBuffer;
	printf("How many words would you like to enter?: ");
	inputBuffer = (char*)malloc(10 * sizeof(char));
	fgets(inputBuffer, 10, stdin);
	const size_t amt = atoi(inputBuffer);
	const char * userWords[amt];

*/
	printf("How many words would you like to enter?: ");
	char * inputBuffer = (char*)malloc(10 * sizeof(char));
	fgets(inputBuffer, 10, stdin);
	int amt = atoi(inputBuffer);
	const size_t newAmt = const_cast<int&>(amt);
	const char * userWords[newAmt];

	_getch();

#if defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif // _DEBUG


}

char * Sanitize(char* in) {
	int place = 0;
	while (in[place] != '\n' && in[place] != '\0') {
		place++;
	}
	if (in[place] != '\0')
		in[place] = ' ';

	return in;
}

char * GetUserInput()
{

	size_t wordIndex = 0;
	size_t numberOfWords = 1;
	char * userWords;

	bool gettingInput = true;

	while (gettingInput) {
		char * inputBuffer = (char*)malloc(256 * sizeof(char));

		printf("Please enter a word: ");
		fgets(inputBuffer, 256, stdin);

		if (inputBuffer[0] == '\n')
			continue;
		inputBuffer = Sanitize(inputBuffer);
		userWords[wordIndex] = *inputBuffer;
		wordIndex++;
		numberOfWords++;
	}


}

char * MakeSentence(const char * input[]) {

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

	//int spacesNeeded = amountOfWords-1; //amount of words - 1
	//int totalNeeded = spacesNeeded + numberOfChars+2; //including period and null terminator
	size_t totalNeeded = numberOfChars + 2;

	char* ptr = (char*)malloc(totalNeeded);

	//create sentence
	amountOfWords = 0;
	currentCharIndex = 0;
	int place = 0;
	while (input[amountOfWords] != NULL) {

		while (input[amountOfWords][currentCharIndex] != '\0') {
			if (input[amountOfWords][currentCharIndex] == ' ')
				break;
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
