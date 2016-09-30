#include "MonsterDebug.h"
#include <string.h>
#include <stdlib.h>

void MonsterDebug::DebugPrint(const char * format, ...)
{
	const size_t tempSize = 256;
	char tempString[tempSize] = "\n==DEBUG==\n";

	//stick args on
	strcat_s(tempString, format);

	//put new line
	//strcat_s(tempString, "\n");

	//define new buffer
	const size_t finalOutputSize = tempSize + 32;

	char * finalOutput = (char*) malloc(finalOutputSize * sizeof(char));

	//store list of arguments
	va_list arguments;

	//initializes arguments to store all values after num
	va_start(arguments, format);

	//vsprintf_s(buffer, len, format, args);
	vsprintf_s(finalOutput, finalOutputSize, tempString, arguments);
	puts(finalOutput);
	free(finalOutput);
	/*for (int i = 0; format[i] != '\0'; i++) {

		if (format[i]== 's') {
			printf("%s\n", va_arg(arguments, char *));
		}

	}*/

	va_end(arguments);
	/*
	for (int i = 0; input[i] != '\0'; i++) {
	printf
	}*/
}
