#pragma once
#include <stdlib.h>
#include <time.h>
#include <math.h>
namespace MMEngine {

	class MMath
	{
	public:
		static int GetRandomIntInBounds(int min, int max);
		static bool IsNAN(float val);
		static bool IsZero(float val);
		static bool AreEqual(float i_lhs, float i_rhs);
		static bool AreEqual_Eps(float i_lhs, float i_rhs, float i_maxDiff);
	};

}




