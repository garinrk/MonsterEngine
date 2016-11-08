#include "MonsterPoint2D.h"


MonsterPoint2D::MonsterPoint2D(double init_x, double init_y) :
	coord_x_(init_x),
	coord_y_(init_y)
{
}


void MonsterPoint2D::Normalize() {
	coord_x_ = coord_x_ / Magnitude();
	coord_y_ = coord_y_ / Magnitude(); 
}

#include "MonsterPoint2D-inl.h"


