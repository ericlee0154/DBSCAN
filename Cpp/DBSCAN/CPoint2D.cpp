#include <iostream>
#include "Geometry.h"

ds::CPoint2D::CPoint2D(void): x(0.0F), y(0.0F), clusterID(0), visited(false) { }
ds::CPoint2D::CPoint2D(float x, float y, int clusterID, bool visited):x(x), y(y), clusterID(clusterID), visited(visited) { }