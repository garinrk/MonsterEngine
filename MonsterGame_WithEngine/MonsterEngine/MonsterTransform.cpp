#include "MonsterTransform.h"



MonsterTransform::MonsterTransform(double new_x, double new_y) :
	position_(new_x, new_y)
{
}


MonsterTransform::~MonsterTransform()
{
}

void MonsterTransform::Translate(double delta_x, double delta_y)
{
	position_.x(position_.x() + delta_x);
	position_.y(position_.y() + delta_y);
}

void MonsterTransform::SetPosition(const MonsterPoint2D& new_pos)
{
	position_.x(new_pos.x());
	position_.y(new_pos.y());
}

