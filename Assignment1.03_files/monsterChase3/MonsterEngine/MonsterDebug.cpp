#include "MonsterDebug.h"
#include <string.h>
#include <stdlib.h>
#include <Windows.h>

void MonsterDebug::DebugPrint(const char * format, ...)
{
	const size_t tempSize = 256;
	char tempString[tempSize] = "===\nDEBUGLOG: ";

	//stick args on
	strcat_s(tempString, format);

	//define new buffer
	const size_t finalOutputSize = tempSize + 32;

	char * finalOutput = (char*) malloc(finalOutputSize * sizeof(char));

	//store list of arguments
	va_list arguments;

	//initializes arguments to store all values
	va_start(arguments, format);

	//copy buffer
	vsprintf_s(finalOutput, finalOutputSize, tempString, arguments);
	
	//send to VS output window
	OutputDebugStringA(finalOutput);
	free(finalOutput);

	va_end(arguments);
}
