/*******************************************************************
 * FILE: ObjectCluster.h
 *------------------------------------------------------------------
 *    DESCRIPTION:
 *      Cluster 2D map with DBSCAN
 * 
 *    LAST EDITED BY:
 *      M.J.Li  (ericlee0154)
 * 
 * ****************************************************************/
#ifndef OBJECTCLUSTER_H
#define OBJECTCLUSTER_H

#include <vector>
#include <algorithm>
#include <math.h>

#include "Geometry.h"

#define FLT_MAX_NUM __FLT_MAX__

class ObjectCluster{
  public:
    ObjectCluster(void);
    ObjectCluster(const unsigned int min_pts_ip,const float eps_ip,const std::vector<CPoint2D> points_ip);

    virtual ~ObjectCluster() = default;
    
    unsigned int DBSCAN();
    void ExpandCluster(const int& clusterID, std::vector<unsigned int>& neighbor_pts_id);
    void RegionQuery(std::vector<unsigned int>& neighbor_pts_id, const CPoint2D& current_point, const unsigned int current_point_id);       // Calculate neighbor points' indices
    // potential functions
    void LabelObjects();
    void DrawObjectsContour();

    static inline float CalculateDistance (const CPoint2D& currentPoint, const CPoint2D& targetPoint);
    static inline void CalculateCenterPt (const CPoint2D& currentPoint, Point2D& centerPoint, float& weight);

    // Call functions
    unsigned int GetNumofPoints() const { return points.size(); }
    float GetEpsilon() const { return eps; }
    const std::vector<CPoint2D>& GetPoints() const { return points; }
    const std::vector<std::vector<unsigned int> >& GetObjectSortedID() const { return object_sorted_id; }
    const std::vector<std::vector<Point2D> >& GetObjectBoundingBoxPoints() const { return objectBBPts; }


  private:
    unsigned int min_pts;    // Minimum points in a cluster
    float eps;  // Minimum distance between points


    std::vector<CPoint2D> points;  // Data points
    std::vector<std::vector<unsigned int> > object_sorted_id; // Sorted Objects' ID
    std::vector<std::vector<Point2D> > objectBBPts;   // Return bounding boxes' points
};

#endif // OBJECTCLUSTER_H