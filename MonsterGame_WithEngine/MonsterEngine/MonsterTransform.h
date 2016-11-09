#ifndef MONSTERTRANSFORM_H
#define MONSTERTRANSFORM_H

#include "MonsterPoint2D.h"
class MonsterTransform
{
public:
	MonsterTransform(double new_x, double new_y);
	~MonsterTransform();
	inline const MonsterPoint2D& GetPosition() const;
	void Translate(const double delta_x, const double delta_y);
private:
	MonsterPoint2D position_;
};

#include "MonsterTransform-inl.h"

#endif // !MONSTERTRANSFORM_H
