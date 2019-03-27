/*******************************************************************
 * FILE: Geometry.h
 *------------------------------------------------------------------
 *    DESCRIPTION:
 *      General definition and usage of geometry.
 * 
 *    LAST EDITED BY:
 *      M.J.Li  (ericlee0154)
 * 
 * ****************************************************************/
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <cmath>
#include <list>
#include <stdio.h>
#include <vector>

#define tolerance 1.0e-06
/******************************
 * CLASS
 *****************************/
namespace geo{
  
  class Point2D;
  class Point3D;
  class CPoint2D;     // Clustered Point2D class

  class Polynomial3;

  namespace vec{
    class Vector2D;
  }
}

namespace geo{  // namespace geo (GEOMETRY)

  class Point2D
  {
    private:
      double x;
      double y;

    public:
      Point2D(void);
      Point2D(double x, double y);
      virtual ~Point2D() = default;

      // Define general Point2D's operators
      friend bool operator==(const Point2D& p1, const Point2D& p2);
      friend bool operator!=(const Point2D& p1, const Point2D& p2);
      friend Point2D operator+(const Point2D& p1, const Point2D& p2);
      friend Point2D operator-(const Point2D& p1, const Point2D& p2);


      void SetX(double x_input) { x = x_input; }
      void SetY(double y_input) { y = y_input; }

      double GetX() const { return x; }
      double GetY() const { return y; }
  };


  class Point3D
  {
    private:
      double x;
      double y;
      double z;

    public:
      Point3D(void);
      Point3D(double x, double y);
      virtual ~Point3D() = default;

      // Define general Point3D's operators
      friend bool operator==(const Point3D& p1, const Point3D& p2);
      friend bool operator!=(const Point3D& p1, const Point3D& p2);
      friend Point3D operator+(const Point3D& p1, const Point3D& p2);
      friend Point3D operator-(const Point3D& p1, const Point3D& p2);


      void SetX(double x_input) { x = x_input; }
      void SetY(double y_input) { y = y_input; }
      void SetZ(double z_input) { z = z_input; }

      double GetX() const { return x; }
      double GetY() const { return y; }
      double GetZ() const { return z; }
  };


  class CPoint2D : public Point2D
  {
    private:
      double x;
      double y;
      int cluster_id;      // -1: noise points

    public:
      CPoint2D(void);
      CPoint2D(int cluster_id_input);
      virtual ~CPoint2D() = default;

      void SetClusterID(int cluster_id_input);

      int GetClusterID() const { return cluster_id; }
  };


  class Polynomial3
  {
    private:
      double a0;
      double a1;
      double a2;
      double a3;

    public:
      Polynomial3(void);
      Polynomial3(double a0, double a1, double a2, double a3);
      virtual ~Polynomial3() = default;

      // Define general operators
      friend bool operator==(const Polynomial3& poly1, const Polynomial3& poly2);
      friend bool operator!=(const Polynomial3& poly1, const Polynomial3& poly2);

      double GetFuncValue(const double& x_input);
      Polynomial3 GetDerivative(const Polynomial3& poly0);
      
      double GetA0() const { return a0; }
      double GetA1() const { return a1; }
      double GetA2() const { return a2; }
      double GetA3() const { return a3; }

      void SetA0(double a0_input) { a0 = a0_input; }
      void SetA1(double a1_input) { a1 = a1_input; }
      void SetA2(double a2_input) { a2 = a2_input; }
      void SetA3(double a3_input) { a3 = a3_input; }
      
  };

  namespace vec{  // namespace vec (VECTOR)

    class Vector2D
    {
      private:
        double x;
        double y;
        double theta;
      
      public:
        Vector2D(void);
        Vector2D(double x_input, double y_input, double theta_input);
        virtual ~Vector2D() = default;

        // Define general Vector2D's operators
        friend bool operator==(const Vector2D& v1, const Vector2D& v2);
        friend bool operator!=(const Vector2D& v1, const Vector2D& v2);
        friend Vector2D operator+(const Vector2D& v1, const Vector2D& v2);
        friend Vector2D operator-(const Vector2D& v1, const Vector2D& v2);
        friend double operator*(const Vector2D& v1, const Vector2D& v2);  // inner product

        void Normalized(const Vector2D& v0);
        double Norm(const Vector2D& v0);

        double GetX() const { return x; }
        double GetY() const { return y; }
        double GetTheta() const { return theta; }

        void SetX(double x_input) { x = x_input; }
        void SetY(double y_input) { y = y_input; }
        void SetTheta(double theta_input) { theta = theta_input; }

    };

    // class Vector3D   or EulerAngle?

  } // namespace vec

} // namespace geo











#endif  // GEOMETRY_H