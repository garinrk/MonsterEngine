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

#define MAXWORDCOUNT 100

char * MakeSentence(char ** input);
char * Sanitize(char* in);


void main(int i_argc, char ** i_argl) {

	int wordsEntered = 0;
	char ** userWords = (char**)malloc(MAXWORDCOUNT * sizeof(char*));
	
	while (wordsEntered < MAXWORDCOUNT) {
		userWords[wordsEntered] = (char*)malloc(256 * sizeof(char));
		printf("Please enter a word (100 WORD MAXIMUM)[ENTER TO END]: ");
		fgets(userWords[wordsEntered], 256, stdin);
		
		if (userWords[wordsEntered][0] == '\n') {
			free(userWords[wordsEntered]);
			userWords[wordsEntered] = NULL;
			
			break;
		}
		else {
			Sanitize(userWords[wordsEntered]);
		}
	
		wordsEntered++;
	}

	char * result = MakeSentence(userWords);

	printf("The sentence is : %s" , result);

	free(result);

	for (int i = 0; i < wordsEntered+1; i++) {
		if (userWords[i] != NULL) {
			free(userWords[i]);
		}
			
	}

	free(userWords);
	//printf("How many words would you like to enter?: ");
	//char * inputBuffer = (char*)malloc(10 * sizeof(char));
	//fgets(inputBuffer, 10, stdin);
	//int amt = atoi(inputBuffer);
	//if (amt <= 0)
	//	return;
	//free(inputBuffer);
	//char ** words = (char**)malloc((amt + 1) * sizeof(char*));


	//for (int i = 0; i < amt; i++) {
	//	printf("Please enter a word [ENTER TO END]: ");
	//	words[i] = (char*)malloc(256 * sizeof(char));
	//	fgets(words[i], sizeof(words[i]), stdin);

	//	Sanitize(words[i]);
	//}

	//words[amt] = NULL;

	//char * result = MakeSentence(words);

	//printf("The Sentence is: %s", result);

	//free(result);

	//for (int i = 0; i, i < amt; i++) {
	//	if (words[i] != NULL) {
	//		free(words[i]);
	//	}
	//		
	//}

	//free(words);
	_getch();



#ifdef _CRTDBG_MAP_ALLOC
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

char * MakeSentence(char ** input) {

	int currentCharIndex = 0;
	int amountOfWords = 0;
	int numberOfChars = 0;

	while (input[amountOfWords] != NULL) {
		char current = input[amountOfWords][currentCharIndex];
		while (current != '\0') {
			numberOfChars++;
			currentCharIndex++;
			current = input[amountOfWords][currentCharIndex];

		}
		amountOfWords++;
		currentCharIndex = 0;

	}
	size_t totalNeeded = numberOfChars +1;

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
