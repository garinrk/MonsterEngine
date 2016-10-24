#include "MMath.h"

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

int MMath::GetRandomIntInBounds(int min, int max)
{
	srand((unsigned int)time(NULL));
	return min + rand() % (max - min + 1);
}
