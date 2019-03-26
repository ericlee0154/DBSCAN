#include <iostream>
#include "Geometry.h"

Point2D::Point2D(void): x(0.0F), y(0.0F) { }
Point2D::Point2D(float x, float y):x(x), y(y) { }

void Point2D::SetX(float x_ip)
{ 
    x = x_ip;
}

void Point2D::SetY(float y_ip)
{ 
    y = y_ip;
}