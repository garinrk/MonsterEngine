#include "MonsterPoint2D.h"


MonsterPoint2D::MonsterPoint2D(float i_x, float i_y) :
	coord_x_(i_x),
	coord_y_(i_y)
{
	assert(!MMEngine::MMath::IsNAN(coord_x_));
	assert(!MMEngine::MMath::IsNAN(coord_y_));
}


void MonsterPoint2D::Normalize() {
	coord_x_ = coord_x_ / Magnitude();
	coord_y_ = coord_y_ / Magnitude(); 
}

#include "MonsterPoint2D-inl.h"


