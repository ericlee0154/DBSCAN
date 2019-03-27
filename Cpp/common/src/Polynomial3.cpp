#include "Geometry.h"

namespace geo{

  Polynomial3::Polynomial3(void): a0(0.0), a1(0.0), a2(0.0), a3(0.0) { }
  Polynomial3::Polynomial3(double a0, double a1, double a2, double a3):a0(a0), a1(a1), a2(a2), a3(a3) { }

  // Define general operators
  bool operator==(const Polynomial3& poly1, const Polynomial3& poly2)
  {
    return ( (std::abs(poly1.GetX() - poly2.GetX()) < tolerance) && (std::abs(poly1.GetY() - poly2.GetY()) < tolerance) );
  }

  bool operator!=(const Polynomial3& poly1, const Polynomial3& poly2)
  {
    return !(poly1 == poly2);
  }

  double GetFuncValue(const double& x_input)
  {
    return (a0 + a1*x_input + a2*x_input*x_input + a3*x_input*x_input*x_input);
  }

  Polynomial3 GetDerivative(const Polynomial3& poly0)
  {
    return ( Polynomial3(a1, a2*2.0, a3*3.0, 0.0) );
  }
} // namespace geo