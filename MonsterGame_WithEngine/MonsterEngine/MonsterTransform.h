#pragma once

#include "MonsterPoint2D.h"
class MonsterTransform
{
public:
	MonsterTransform(float i_x, float i_y);
	~MonsterTransform();
	inline const MonsterPoint2D& GetPosition() const;
	void SetPosition(const MonsterPoint2D& i_pos);
	void Translate(const float i_deltaX, const float i_deltaY);
private:
	MonsterPoint2D position_;
};

#include "MonsterTransform-inl.h"
