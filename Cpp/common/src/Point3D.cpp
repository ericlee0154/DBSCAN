#include "Geometry.h"

namespace geo{

  Point2D::Point3D(void): x(0.0), y(0.0), z(0.0) { }
  Point2D::Point2D(double x, double y):x(x), y(y), z(z) { }

  // Define general operators
  bool operator==(const Point3D& p1, const Point3D& p2)
  {
    return ( (std::abs(p1.GetX() - p2.GetX()) < tolerance) && (std::abs(p1.GetY() - p2.GetY()) < tolerance) && (std::abs(p1.GetZ() - p2.GetZ()) < tolerance) );
  }

  bool operator!=(const Point3D& p1, const Point3D& p2)
  {
    return !(p1 == p2);
  }

  Point3D operator+(const Point3D& p1, const Point3D& p2)
  {
    return ( Point3D(p1.GetX()+p2.GetX(), p1.GetY()+p2.GetY(), p1.GetZ()+p2.GetZ()) );
  }

  Point3D operator-(const Point3D& p1, const Point3D& p2)
  {
    return ( Point3D(p1.GetX()-p2.GetX(), p1.GetY()-p2.GetY(), p1.GetZ()-p2.GetZ()) );
  }
} // namespace geo