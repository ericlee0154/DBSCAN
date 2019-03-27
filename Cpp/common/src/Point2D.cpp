#include "Geometry.h"

namespace geo{

  Point2D::Point2D(void): x(0.0), y(0.0) { }
  Point2D::Point2D(double x, double y):x(x), y(y) { }

  // Define general operators
  bool operator==(const Point2D& p1, const Point2D& p2)
  {
    return ( (std::abs(p1.GetX() - p2.GetX()) < tolerance) && (std::abs(p1.GetY() - p2.GetY()) < tolerance) );
  }

  bool operator!=(const Point2D& p1, const Point2D& p2)
  {
    return !(p1 == p2);
  }

  Point2D operator+(const Point2D& p1, const Point2D& p2)
  {
    return ( Point2D(p1.GetX()+p2.GetX(), p1.GetY()+p2.GetY()) );
  }

  Point2D operator-(const Point2D& p1, const Point2D& p2)
  {
    return ( Point2D(p1.GetX()-p2.GetX(), p1.GetY()-p2.GetY()) );
  }
} // namespace geo