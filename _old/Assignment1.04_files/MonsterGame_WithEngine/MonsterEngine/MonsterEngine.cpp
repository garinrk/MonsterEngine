#include "MonsterEngine.h"

#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Random true or false. </summary>
///
/// <remarks>	garinrk, 10/23/2016. </remarks>
///
/// <returns>	An int, 1 = true. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////

int MonsterEngine::RandomTrueOrFalse()
{
	srand((unsigned int)time(NULL));
	return rand() % 2;
}

