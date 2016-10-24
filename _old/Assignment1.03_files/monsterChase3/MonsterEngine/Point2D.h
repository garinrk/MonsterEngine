#ifndef POINT2D_H
#define POINT2D_H

#include <math.h>

class Point2D
{
public:
	Point2D();
	double Magnitude();
	double DotProduct(Point2D other);
	void Normalize();
	double x()
	{
		return coord_x;
	};
	double y()
	{
		return coord_y;
	}
	void x(double val) {
		coord_x = val;
	}
	void y(double val) {
		coord_y = val;
	}

	Point2D operator+(Point2D rhs);
	Point2D operator-(Point2D rhs);

	/*X& X::operator=(X rhs)
	{
		swap(rhs);
		return *this;
	}*/

private:
	double coord_x;
	double coord_y;
};

#endif

