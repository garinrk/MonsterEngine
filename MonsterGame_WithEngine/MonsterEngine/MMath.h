#pragma once
#include <stdlib.h>
#include <time.h>
#include <math.h>
class MMath
{
public:
	static int GetRandomIntInBounds(int min, int max);
	static bool IsNAN(float val);
	inline bool IsZero(float val);
	inline bool AreEqual_Eps(float i_lhs, float i_rhs, float i_maxDiff);
	//TODO: need absolute value fabs
};




