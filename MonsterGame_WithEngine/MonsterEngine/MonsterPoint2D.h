#ifndef POINT2D_H
#define POINT2D_H

#include <math.h>
#include "MMath.h"
#include "assert.h"
class MonsterPoint2D
{
public:
	MonsterPoint2D(float init_x, float init_y);
	inline float Magnitude() const;
	inline float DotProduct(const MonsterPoint2D& other) const;
	void Normalize();
	inline float x() const;
	inline float y() const;
	inline void x(const float val);
	inline void y(const float val);

	inline MonsterPoint2D operator+(const MonsterPoint2D& rhs) const;
	inline MonsterPoint2D operator-(const MonsterPoint2D& rhs) const;


private:
	float coord_x_;
	float coord_y_;
};
#include "MonsterPoint2D-inl.h"
#endif


