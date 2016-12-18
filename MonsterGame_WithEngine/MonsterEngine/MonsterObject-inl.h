#pragma once

inline void MonsterObject::SetTransform(const MonsterTransform &o_transform)
{
	transform_ = o_transform;
}

inline const MonsterTransform& MonsterObject::GetTransform() const
{
	return transform_;
}

inline void MonsterObject::ObjectTranslate(const MonsterPoint2D &i_delta)
{
	transform_.Translate(i_delta.x(), i_delta.y());
}
