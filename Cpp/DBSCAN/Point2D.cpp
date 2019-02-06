#include <iostream>
#include "Geometry.h"

ds::Point2D::Point2D(void): x(0.0F), y(0.0F) { }
ds::Point2D::Point2D(float x, float y):x(x), y(y) { }

void ds::Point2D::SetValue(float x_ip, float y_ip)
{ 
    x = x_ip;
    y = y_ip;
}