#ifndef MONSTERPOINT2D_INL_H
#define MONSTERPOINT2D_INL_H

inline float MonsterPoint2D::x() const {
	return coord_x_;
}

inline float MonsterPoint2D::y() const {
	return coord_y_;
}

inline void MonsterPoint2D::x(const float val) {
	assert(MMath::IsNAN(val));
	coord_x_ = val;
}

inline void MonsterPoint2D::y(const float val)
{
	assert(MMath::IsNAN(val));
	coord_y_ = val;
}

inline float MonsterPoint2D::Magnitude() const {
	return static_cast<float>(sqrt((coord_x_*coord_x_) + (coord_y_*coord_y_)));
}


inline float MonsterPoint2D::DotProduct(const MonsterPoint2D& other) const {
	return coord_x_ * other.x() + coord_y_ * other.y();
}

inline MonsterPoint2D MonsterPoint2D::operator+(const MonsterPoint2D& rhs) const {
	return MonsterPoint2D(rhs.x() + x(), rhs.y() + y());
}

inline MonsterPoint2D MonsterPoint2D::operator-(const MonsterPoint2D& rhs) const {
	return MonsterPoint2D(rhs.x() - x(), rhs.y() - y());
}

#endif