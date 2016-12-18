#include "MonsterTransform.h"



MonsterTransform::MonsterTransform(float i_x, float i_y) :
	position_(i_x, i_y)
{
	assert(!MMEngine::MMath::IsNAN(i_x));
	assert(!MMEngine::MMath::IsNAN(i_y));
}


MonsterTransform::~MonsterTransform()
{
}

void MonsterTransform::Translate(float i_deltaX, float i_deltaY)
{
	position_.x(position_.x() + i_deltaX);
	position_.y(position_.y() + i_deltaY);
}

void MonsterTransform::SetPosition(const MonsterPoint2D& i_pos)
{
	position_.x(i_pos.x());
	position_.y(i_pos.y());
}

