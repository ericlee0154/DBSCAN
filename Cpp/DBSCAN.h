/*******************************************************************
 * FILE: DBSCAN.h
 *------------------------------------------------------------------
 *    DESCRIPTION:
 *      Density-Based Spatial Clustering and Noise
 * 
 *    LAST EDITED BY:
 *      M.J.Li  (ericlee0154)
 * 
 * ****************************************************************/
#ifndef DBSCAN_H
#define DBSCAN_H

#include <vector>
#include <algorithm>
#include <math.h>

#include "Geometry.h"


class DBSCAN{
  public:
    DBSCAN(void);
    DBSCAN(const unsigned int min_pts_ip,const double eps_ip,const std::vector<geo::CPoint2D> points_ip);

    virtual ~DBSCAN() = default;
    
    void ExpandCluster(const int& clusterID, std::vector<unsigned int>& neighbor_pts_id);
    void RegionQuery(std::vector<unsigned int>& neighbor_pts_id, const geo::CPoint2D& current_point, const unsigned int current_point_id);

    std::vector<geo::CPoint2D> GetPoints() const { return points; }

  private:
    unsigned int min_pts; // Minimum points in a cluster
    double eps;  // Minimum distance between points
    std::vector<geo::CPoint2D> points; // Data points
};

#endif // DBSCAN_H