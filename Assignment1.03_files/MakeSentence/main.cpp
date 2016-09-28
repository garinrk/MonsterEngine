#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

char * test(const char ** input);

void main(int i_argc, char ** i_argl) {
	printf("Hello world!");
	const char * strings[] = {
		"This",
		"is",
		"a",
		"test",
		NULL
	};
	char * result = test(strings);

	printf("The Sentence is: %s", result);

	free(result);

	_getch();
}

char * test(const char ** input) {

	char * r;
	
	char currentChar = 0;
	int currentWord = 0;

	int numberOfChars = 0;

	while (input[currentWord] != NULL) {
		while (input[currentWord][currentChar] != '\0') {
			numberOfChars++;
			currentChar++;

		}
		currentWord++;
		currentChar = 0;

	}

	_getch();

	return r;

}
