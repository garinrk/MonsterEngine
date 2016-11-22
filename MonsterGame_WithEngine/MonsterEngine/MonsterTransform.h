#ifndef MONSTERTRANSFORM_H
#define MONSTERTRANSFORM_H

#include "MonsterPoint2D.h"
class MonsterTransform
{
public:
	MonsterTransform(float new_x, float new_y);
	~MonsterTransform();
	inline const MonsterPoint2D& GetPosition() const;
	void SetPosition(const MonsterPoint2D& new_pos);
	void Translate(const float delta_x, const float delta_y);
private:
	MonsterPoint2D position_;
};

#include "MonsterTransform-inl.h"

#endif // !MONSTERTRANSFORM_H
