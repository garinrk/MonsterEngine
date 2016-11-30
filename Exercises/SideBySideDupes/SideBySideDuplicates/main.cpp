#include <conio.h>  
#include <ctype.h>  
#include <iostream>
#include <stdio.h>
#include <string>


std::string ReverseString(std::string i_phrase);
//Remove side-by-side duplicate characters in a string:
//("AAA BBB" becomes "A B") in place.

int main(int i_argc, char ** i_argl) {

	std::string input = "Richards";

	std::string r = ReverseString(input);

	std::cout << "Result: " << r << std::endl;

	return 0;
}


std::string ReverseString(std::string o_phrase) {

	std::cout << "Reversing: " << o_phrase << std::endl;
	char temp;
	size_t front = 0;
	size_t back = o_phrase.length() - 1;

	while(true){
		temp = o_phrase[front];

		o_phrase[front] = o_phrase[back];

		o_phrase[back] = temp;

		front++;
		back--;

		if (front == back)
			break;
	}

	return o_phrase;
	
}