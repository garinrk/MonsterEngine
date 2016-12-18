#ifndef MONSTERPOINT2D_INL_H
#define MONSTERPOINT2D_INL_H

inline float MonsterPoint2D::x() const {
	return coord_x_;
}

inline float MonsterPoint2D::y() const {
	return coord_y_;
}

inline void MonsterPoint2D::x(const float i_val) {
	assert(!MMEngine::MMath::IsNAN(i_val));
	coord_x_ = i_val;
}

inline void MonsterPoint2D::y(const float i_val)
{
	assert(!MMEngine::MMath::IsNAN(i_val));
	coord_y_ = i_val;
}

inline float MonsterPoint2D::Magnitude() const {
	return static_cast<float>(sqrt((coord_x_*coord_x_) + (coord_y_*coord_y_)));
}


inline float MonsterPoint2D::DotProduct(const MonsterPoint2D& i_other) const {
	return coord_x_ * i_other.x() + coord_y_ * i_other.y();
}

inline MonsterPoint2D MonsterPoint2D::operator+(const MonsterPoint2D& i_rhs) const {
	return MonsterPoint2D(i_rhs.x() + x(), i_rhs.y() + y());
}

inline MonsterPoint2D MonsterPoint2D::operator-(const MonsterPoint2D& i_rhs) const {
	return MonsterPoint2D(i_rhs.x() - x(), i_rhs.y() - y());
}

#endif