#include "Point2D.h"


Point2D::Point2D(double init_x, double init_y) :
	coord_x_(init_x),
	coord_y_(init_y)
{
}


void Point2D::Normalize() {
	coord_x_ = coord_x_ / Magnitude();
	coord_y_ = coord_y_ / Magnitude(); 
}

#include "Point2D-inl.h"


