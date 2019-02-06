#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <iostream>

/******************************
 * CLASS
 *****************************/
namespace ds{
class Point2D;
class CPoint2D;     // Clustered Point2D class


class Point2D
{
  private:
    float x;
    float y;

  public:
    Point2D(void);
    Point2D(float x, float y);
    ~Point2D() {};

    void SetX(float x_ip);
    void SetY(float y_ip);

    float GetX() const { return x; }
    float GetY() const { return y; }
};


class CPoint2D
{
  private:
    float x;
    float y;
    int clusterID;      // -1: noise points
    bool visited;

  public:
    CPoint2D(void);
    CPoint2D(float x, float y, int clusterID, bool visited);
    ~CPoint2D() {};

    void SetX(float x_ip);
    void SetY(float y_ip);
    void SetClusterID(int clusterID_ip);
    void SetVisited(bool visited_ip);

    // float GetX () {return x;}
    float GetX() const { return x; }
    float GetY() const { return y; }
    int GetClusterID() const { return clusterID; }
    bool GetVisited() const { return visited; }
};

}


#endif  // _GEOMETRY_H_