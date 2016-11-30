#include <conio.h>  
#include <ctype.h>  
#include <iostream>
#include <stdio.h>
#include <string>

void ReverseCharArray(char*  i_phrase);

int main(int i_argc, char ** i_argl) {
	
	char input2[6] = { 'g','a', 'r','i','n', '\0' };

	std::cout << "Reversing " << input2 << std::endl;
	ReverseCharArray(input2);

	std::cout << "Result: " << input2 << std::endl;

	return 0;
}

void ReverseCharArray(char * i_phrase) {

	//get to the end

	size_t backIndex = 0;
	size_t frontIndex = 0;

	while (i_phrase[backIndex + 1] != '\0')
		backIndex++;

	char front, back;
	while (true) {
		front = i_phrase[frontIndex];
		back = i_phrase[backIndex];

		if (front == back)
			break;

		i_phrase[frontIndex] = back;
		i_phrase[backIndex] = front;

		frontIndex++;
		backIndex--;
	}
}