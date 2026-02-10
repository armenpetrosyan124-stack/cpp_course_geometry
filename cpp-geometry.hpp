#include <iostream>
#include <vector>

using std::vector;

class Vector {
 protected:
  int64_t x_;
  int64_t y_;

 public:
  Vector();
  Vector(int64_t, int64_t);

  int64_t operator*(const Vector&) const;
  int64_t DotProduct(const Vector&) const;

  int64_t operator^(const Vector&) const;
  int64_t CrossProduct(const Vector&) const;

  Vector& operator+=(const Vector&);
  Vector& operator-=(const Vector&);
  Vector& operator*=(int64_t);
  Vector operator-() const;

  int64_t GetX() const;
  int64_t GetY() const;
  int64_t GetNormSquared() const;
};

Vector operator+(const Vector&, const Vector&);
Vector operator-(const Vector&, const Vector&);
Vector operator*(const Vector&, int64_t);
Vector operator*(int64_t, const Vector&);

class Point;
class Segment;

class IShape {
 public:
  virtual void Move(const Vector&) = 0;
  virtual bool ContainsPoint(const Point&) const = 0;
  virtual bool CrossSegment(const Segment&) const = 0;
  virtual IShape* Clone() const = 0;
  virtual ~IShape() {}
};

class Point : public IShape, public Vector {
 public:
  Point();
  Point(int64_t, int64_t);
  Point(const Vector&);

  bool operator==(const Point&) const;

  void Move(const Vector& vec);
  bool ContainsPoint(const Point& other) const;
  bool CrossSegment(const Segment& seg) const;
  Point* Clone() const;
};

class Segment : public IShape {
 private:
  Point a_;
  Point b_;

 public:
  Segment(const Point&, const Point&);
  Point GetA() const;
  Point GetB() const;

  void Move(const Vector& vec);
  bool ContainsPoint(const Point& point) const;
  bool CrossSegment(const Segment& other) const;
  Segment* Clone() const;
};

class Line : public IShape {
 private:
  int64_t a_;
  int64_t b_;
  int64_t c_;

 public:
  Line(const Point&, const Point&);
  int64_t GetA() const;
  int64_t GetB() const;
  int64_t GetC() const;

  int64_t Substitute(const Point&) const;

  void Move(const Vector& vec);
  bool ContainsPoint(const Point& point) const;
  bool CrossSegment(const Segment& seg) const;
  Line* Clone() const;
};

class Ray : public IShape {
 private:
  Point a_;
  Point v_;

 public:
  Ray(const Point&, const Point&);
  Point GetA() const;
  Point GetVector() const;

  void Move(const Vector& vec);
  bool ContainsPoint(const Point& point) const;
  bool CrossSegment(const Segment& seg) const;
  Ray* Clone() const;
};

class Circle : public IShape {
 private:
  Point c_;
  int64_t r_;

 public:
  Circle(const Point&, int64_t);

  Point GetCentre() const;
  int64_t GetRadius() const;

  void Move(const Vector& vec);
  bool ContainsPoint(const Point& point) const;
  bool CrossSegment(const Segment& seg) const;
  Circle* Clone() const;
};
