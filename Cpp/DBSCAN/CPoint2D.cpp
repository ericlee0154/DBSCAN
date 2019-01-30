#include <iostream>
#include "Geometry.h"

CPoint2D::CPoint2D(void): x(0.0), y(0.0), clusterID(0), visited(false) { }
CPoint2D::CPoint2D(float x, float y, int clusterID, bool visited):x(x), y(y), clusterID(clusterID), visited(visited) { }