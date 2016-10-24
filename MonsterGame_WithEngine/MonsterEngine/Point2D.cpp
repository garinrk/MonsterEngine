#include "Point2D.h"


Point2D::Point2D(double init_x, double init_y)
{
	x(init_x);
	y(init_y);
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

double Point2D::x()
{
	return coord_x;
}

double Point2D::y()
{
	return coord_y;
}

void Point2D::x(double val)
{
	coord_x = val;
}

void Point2D::y(double val)
{
	coord_y = val;
}

Point2D Point2D::operator+(Point2D rhs)
{
	double newX = rhs.x() + x();
	double newY = rhs.y() + y();
	Point2D newPoint = Point2D(newX, newY);

	return newPoint;
}

Point2D Point2D::operator-(Point2D rhs)
{
	double newX = rhs.x() - x();
	double newY = rhs.y() - y();
	Point2D newPoint = Point2D(newX, newY);

	return newPoint;
}



