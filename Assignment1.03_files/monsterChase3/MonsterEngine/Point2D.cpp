#include "Point2D.h"



Point2D::Point2D()
{
	x(0);
	y(0);
}

double Point2D::Magnitude() {
	//magnitude
	double val = coord_x * coord_x + coord_y * coord_y;

	return sqrt(val);
}

double Point2D::DotProduct(Point2D other) {
	//Ax*Bx + Ay*By
	return coord_x * other.x() + coord_y * other.y();	
}

void Point2D::Normalize() {
	coord_x = coord_x / Magnitude();
	coord_y = coord_y / Magnitude(); 
}

Point2D Point2D::operator+(Point2D rhs)
{
	Point2D val;
	val.x(rhs.x() + x());
	val.y(rhs.y() + y());

	return val;
}

Point2D Point2D::operator-(Point2D rhs)
{
	Point2D val;
	val.x(rhs.x() - x());
	val.y(rhs.y() - y());

	return val;
}

