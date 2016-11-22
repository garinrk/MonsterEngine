#ifndef MMATH_H
#define MMATH_H
#include <stdlib.h>
#include <time.h>

class MMath
{
public:
	static int GetRandomIntInBounds(int min, int max);
	static bool IsNAN(float val);
};

#endif



