#include "MonsterEngine.h"

#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <stdio.h>


int MonsterMath::GetRandomIntInBounds(int min, int max)
{
	srand((unsigned int)time(NULL));
	return min + rand() % (max - min + 1);
}

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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Gets random int in bounds. </summary>
///
/// <remarks>	garinrk, 10/23/2016. </remarks>
///
/// <param name="min">	The minimum. </param>
/// <param name="max">	The maximum. </param>
///
/// <returns>	The random int in bounds. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////


