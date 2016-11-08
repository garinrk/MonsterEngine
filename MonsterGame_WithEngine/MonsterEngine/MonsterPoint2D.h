#ifndef POINT2D_H
#define POINT2D_H

#include <math.h>

class MonsterPoint2D
{
public:
	MonsterPoint2D(double init_x, double init_y);
	inline double Magnitude() const;
	inline double DotProduct(const MonsterPoint2D& other) const;
	void Normalize();
	inline double x() const;
	inline double y() const;
	inline void x(const double val);
	inline void y(const double val);

	inline MonsterPoint2D operator+(const MonsterPoint2D& rhs) const;
	inline MonsterPoint2D operator-(const MonsterPoint2D& rhs) const;


private:
	double coord_x_;
	double coord_y_;
};
#include "MonsterPoint2D-inl.h"
#endif


