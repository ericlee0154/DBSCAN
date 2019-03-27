#include "Geometry.h"

namespace geo{
  namespace vec{

    Vector2D::Vector2D(void): x(0.0), y(0.0), theta(0.0) { }
    Vector2D::Vector2D(double x, double y, double theta):x(x), y(y), theta(theta) { }


    // Define general operators
    bool operator==(const Vector2D& v1, const Vector2D& v2)
    {
      return ( (std::abs(v1.GetX() - v2.GetX()) < tolerance) && (std::abs(v1.GetY() - v2.GetY()) < tolerance) && (std::abs(v1.GetTheta() - v2.GetTheta()) < tolerance) );
    }

    bool operator!=(const Vector2D& v1, const Vector2D& v2)
    {
      return !(v1 == v2);
    }

    Vector2D operator+(const Vector2D& v1, const Vector2D& v2)
    {
      return ( Vector2D(v1.GetX()+v2.GetX(), v1.GetY()+v2.GetY(), v1.GetTheta()+v2.GetTheta()) );
    }

    Vector2D operator-(const Vector2D& v1, const Vector2D& v2)
    {
      return ( Vector2D(v1.GetX()-v2.GetX(), v1.GetY()-v2.GetY(), v1.GetTheta()-v2.GetTheta()) );
    }
  } // namespace geo
} // namespace vec