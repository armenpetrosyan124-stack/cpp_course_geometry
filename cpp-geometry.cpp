#include "cpp-geometry.hpp"

using std::cout;

Vector::Vector() : x_(0), y_(0) {}

Vector::Vector(int64_t x_coord, int64_t y_coord) : x_(x_coord), y_(y_coord) {}

int64_t Vector::operator*(const Vector& other) const {
  return x_ * other.x_ + y_ * other.y_;
}

int64_t Vector::DotProduct(const Vector& other) const {
  return x_ * other.x_ + y_ * other.y_;
}

int64_t Vector::operator^(const Vector& other) const {
  return x_ * other.y_ - y_ * other.x_;
}

int64_t Vector::CrossProduct(const Vector& other) const {
  return x_ * other.y_ - y_ * other.x_;
}

Vector& Vector::operator+=(const Vector& other) {
  x_ += other.x_;
  y_ += other.y_;
  return *this;
}

Vector& Vector::operator-=(const Vector& other) {
  x_ -= other.x_;
  y_ -= other.y_;
  return *this;
}

Vector& Vector::operator*=(int64_t factor) {
  x_ *= factor;
  y_ *= factor;
  return *this;
}

Vector Vector::operator-() const { return Vector(-x_, -y_); }

int64_t Vector::GetX() const { return x_; }

int64_t Vector::GetY() const { return y_; }

int64_t Vector::GetNormSquared() const { return x_ * x_ + y_ * y_; }

Vector operator+(const Vector& lhs, const Vector& rhs) {
  Vector tmp = lhs;
  tmp += rhs;
  return tmp;
}

Vector operator-(const Vector& lhs, const Vector& rhs) {
  Vector tmp = lhs;
  tmp -= rhs;
  return tmp;
}

Vector operator*(const Vector& vec, int64_t factor) {
  Vector tmp = vec;
  tmp *= factor;
  return tmp;
}

Vector operator*(int64_t factor, const Vector& vec) {
  Vector tmp = vec;
  tmp *= factor;
  return tmp;
}

Point::Point() {}

Point::Point(int64_t x_coord, int64_t y_coord) : Vector(x_coord, y_coord) {}

Point::Point(const Vector& vec) : Vector(vec) {}

Segment::Segment(const Point& start, const Point& finish)
    : a_(start), b_(finish) {}

Line::Line(const Point& start, const Point& finish)
    : a_(start.GetY() - finish.GetY()),
      b_(finish.GetX() - start.GetX()),
      c_(start.GetX() * finish.GetY() - finish.GetX() * start.GetY()) {}

Ray::Ray(const Point& start, const Point& mid) : a_(start), v_(mid - start) {}

Circle::Circle(const Point& center, int64_t radius) : c_(center), r_(radius) {}

Point Segment::GetA() const { return a_; }

Point Segment::GetB() const { return b_; }

int64_t Line::GetA() const { return a_; }

int64_t Line::GetB() const { return b_; }

int64_t Line::GetC() const { return c_; }

Point Ray::GetA() const { return a_; }

Point Ray::GetVector() const { return v_; }

Point Circle::GetCentre() const { return c_; }

int64_t Circle::GetRadius() const { return r_; }

void Point::Move(const Vector& vec) { *this += vec; }

void Segment::Move(const Vector& vec) {
  a_ += vec;
  b_ += vec;
}

void Line::Move(const Vector& vec) { c_ -= a_ * vec.GetX() + b_ * vec.GetY(); }

void Ray::Move(const Vector& vec) { a_ += vec; }

void Circle::Move(const Vector& vec) { c_ += vec; }

bool Point::operator==(const Point& other) const {
  return x_ == other.x_ && y_ == other.y_;
}

bool Point::ContainsPoint(const Point& other) const { return *this == other; }

bool Segment::ContainsPoint(const Point& point) const {
  Vector dir_a = a_ - point;
  Vector dir_b = b_ - point;
  return dir_a.CrossProduct(dir_b) == 0 && dir_a.DotProduct(dir_b) <= 0;
}

bool Line::ContainsPoint(const Point& point) const {
  return a_ * point.GetX() + b_ * point.GetY() + c_ == 0;
}

bool Ray::ContainsPoint(const Point& point) const {
  Vector dir = point - a_;
  return dir.CrossProduct(v_) == 0 && dir.DotProduct(v_) >= 0;
}

bool Circle::ContainsPoint(const Point& point) const {
  Vector dir = point - c_;
  return dir.GetNormSquared() <= r_ * r_;
}

bool Point::CrossSegment(const Segment& seg) const {
  return seg.ContainsPoint(*this);
}

bool SameSign(int64_t value1, int64_t value2) {
  return (value1 > 0 && value2 > 0) || (value1 < 0 && value2 < 0);
}

bool Segment::CrossSegment(const Segment& other) const {
  if (other.ContainsPoint(a_) || other.ContainsPoint(b_)) {
    return true;
  }
  if (ContainsPoint(other.a_) || ContainsPoint(other.b_)) {
    return true;
  }
  Vector dir = b_ - a_;
  Vector dir_other = other.b_ - other.a_;
  if (dir.CrossProduct(dir_other) == 0) {
    return false;
  }

  Vector to_a = other.a_ - a_;
  Vector to_b = other.b_ - a_;

  if (SameSign(dir.CrossProduct(to_a), dir.CrossProduct(to_b))) {
    return false;
  }

  to_a = a_ - other.a_;
  to_b = b_ - other.a_;
  return !SameSign(dir_other.CrossProduct(to_a), dir_other.CrossProduct(to_b));
}

int64_t Line::Substitute(const Point& point) const {
  return a_ * point.GetX() + b_ * point.GetY() + c_;
}

bool Line::CrossSegment(const Segment& seg) const {
  int64_t value1 = Substitute(seg.GetA());
  int64_t value2 = Substitute(seg.GetB());
  return !SameSign(value1, value2);
}

bool Ray::CrossSegment(const Segment& seg) const {
  Line line(a_, a_ + v_);
  if (!line.CrossSegment(seg)) {
    return false;
  }
  Vector to_a = seg.GetA() - a_;
  Vector to_b = seg.GetB() - a_;
  if (ContainsPoint(seg.GetA()) || ContainsPoint(seg.GetB())) {
    return true;
  }
  return SameSign(to_a.CrossProduct(v_), to_a.CrossProduct(to_b));
}

bool Circle::CrossSegment(const Segment& seg) const {
  Vector to_a = seg.GetA() - c_;
  Vector to_b = seg.GetB() - c_;
  int64_t a_norm_sq = to_a.GetNormSquared();
  int64_t b_norm_sq = to_b.GetNormSquared();
  int64_t r_sq = r_ * r_;
  if (a_norm_sq == r_sq || b_norm_sq == r_sq) {
    return true;
  }
  if (a_norm_sq < r_sq && b_norm_sq < r_sq) {
    return false;
  }
  if (a_norm_sq < r_sq || b_norm_sq < r_sq) {
    return true;
  }
  Line line(seg.GetA(), seg.GetB());
  int64_t value = line.Substitute(c_);
  return value * value <=
         r_sq * (line.GetA() * line.GetA() + line.GetB() * line.GetB());
}

Point* Point::Clone() const { return new Point(*this); }

Segment* Segment::Clone() const { return new Segment(*this); }

Line* Line::Clone() const { return new Line(*this); }

Ray* Ray::Clone() const { return new Ray(*this); }

Circle* Circle::Clone() const { return new Circle(*this); }
