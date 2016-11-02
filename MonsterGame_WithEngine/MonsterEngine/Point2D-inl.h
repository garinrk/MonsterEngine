#pragma once
inline double Point2D::x() const {
	return coord_x_;
}

inline double Point2D::y() const {
	return coord_y_;
}

inline void Point2D::x(const double val) {
	coord_x_ = val;
}

inline void Point2D::y(const double val)
{
	coord_y_ = val;
}

inline double Point2D::Magnitude() const {
	return sqrt((coord_x_*coord_x_) + (coord_y_*coord_y_));
}


inline double Point2D::DotProduct(const Point2D& other) const {
	return coord_x_ * other.x() + coord_y_ * other.y();
}

inline Point2D Point2D::operator+(const Point2D& rhs) const {
	return Point2D(rhs.x() + x(), rhs.y() + y());
}

inline Point2D Point2D::operator-(const Point2D& rhs) const {
	return Point2D(rhs.x() - x(), rhs.y() - y());
}
