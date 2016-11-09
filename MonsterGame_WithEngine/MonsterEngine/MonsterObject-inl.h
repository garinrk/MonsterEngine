#ifndef MONSTEROBJECT_INL_H
#define MONSTEROBJECT_INL_H

inline void MonsterObject::SetTransform(const MonsterTransform &new_transform)
{
	transform_ = new_transform;
}

inline const MonsterTransform& MonsterObject::GetTransform() const
{
	return transform_;
}

inline void MonsterObject::ObjectTranslate(const MonsterPoint2D &delta_change)
{
	transform_.Translate(delta_change.x(), delta_change.y());
}

#endif