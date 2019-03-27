#include "Geometry.h"
namespace geo{

  CPoint2D::CPoint2D(void): cluster_id(0) { }
  CPoint2D::CPoint2D(int cluster_id_input): cluster_id(cluster_id) { }
} // namespace geo