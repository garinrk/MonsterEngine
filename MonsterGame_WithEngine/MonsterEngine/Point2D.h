#ifndef POINT2D_H
#define POINT2D_H

#include <math.h>

class Point2D
{
public:
	Point2D(double init_x, double init_y);
	inline double Magnitude() const;
	inline double DotProduct(const Point2D& other) const;
	void Normalize();
	inline double x() const;
	inline double y() const;
	inline void x(const double val);
	inline void y(const double val);

	inline Point2D operator+(const Point2D& rhs) const;
	inline Point2D operator-(const Point2D& rhs) const;


private:
	double coord_x_;
	double coord_y_;
};
#include "Point2D-inl.h"
#endif


