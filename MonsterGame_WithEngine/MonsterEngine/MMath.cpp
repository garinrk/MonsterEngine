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

bool MMath::IsNAN(float val) {

	//we do this to keep overly smart compilers
	//from optimizing this whole function away
	volatile float v = val;

	return v != v;
}

inline bool MMath::IsZero(float val)
{
	return false;
}

inline bool MMath::AreEqual_Eps(float i_lhs, float i_rhs, float i_maxDiff)
{
	return false;
}
