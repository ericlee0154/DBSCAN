#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <iostream>

/******************************
 * CLASS
 *****************************/
class Point2D;
class CPoint2D;     // Clustered Point2D class

/*
class Point2D
{
  public:
    float x;
    float y;

  public:
    Point2D(void);
    Point2D(float x, float y);
    ~Point2D() {};
};
*/

class CPoint2D
{
  public:
    float x;
    float y;
    int clusterID;      // -1: noise points
    bool visited;

  public:
    CPoint2D(void);
    CPoint2D(float x, float y, int clusterID, bool visited);
    ~CPoint2D() {};

    // float GetX () {return x;}

};


#endif  // _GEOMETRY_H_