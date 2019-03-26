#include <iostream>
#include "Geometry.h"

CPoint2D::CPoint2D(void): x(0.0F), y(0.0F), clusterID(0) { }
CPoint2D::CPoint2D(float x, float y, int clusterID, bool visited):x(x), y(y), clusterID(clusterID) { }

void CPoint2D::SetX(float x_ip)
{
    x = x_ip;
}

void CPoint2D::SetY(float y_ip)
{
    y = y_ip;
}

void CPoint2D::SetClusterID(int clusterID_ip)
{
    clusterID = clusterID_ip;
}