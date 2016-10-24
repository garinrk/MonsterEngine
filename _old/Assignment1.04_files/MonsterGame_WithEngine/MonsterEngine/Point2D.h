#ifndef POINT2D_H
#define POINT2D_H

#include <math.h>

class Point2D
{
public:
	Point2D(double init_x, double init_y);
	double Magnitude();
	double DotProduct(Point2D other);
	void Normalize();
	double x();
	double y();
	void x(double val);
	void y(double val);

	Point2D operator+(Point2D rhs);
	Point2D operator-(Point2D rhs);


private:
	double coord_x;
	double coord_y;
};

#endif

