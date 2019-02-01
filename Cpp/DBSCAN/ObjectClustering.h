#ifndef _OBJECTCLUSTERING_H_
#define _OBJECTCLUSTER_H_

#include <vector>
#include <algorithm>
#include <math.h>

#include "Geometry.h"


class ObjectClustering{
  public:
    ObjectClustering(void);    // Original constructor for DBSCAN
    ObjectClustering(unsigned int minPts_ip, float eps_ip, std::vector<CPoint2D> points_ip);

    ~ObjectClustering() {};   // Original destructor for DBSCAN
    
    void RunObjClusTask();
    void DBSCAN(unsigned int minPts, float eps, std::vector<CPoint2D>& points);
    void ExpandCluster(int *clusterID, std::vector<int>& neighborPtsIndex, std::vector<int>& objectContour);
    std::vector<int> RegionQuery(CPoint2D currentPoint);       // Calculate neighbor points' indices
    // potential functions
    void LabelObjects(std::vector<CPoint2D>& points_, std::vector<std::vector<int>>& objectSortedID);


    inline float CalculateDistance (CPoint2D& currentPoint, CPoint2D& targetPoint);
    inline std::vector<Point2D> CalculateCenterPt (std::vector<std::vector<int>>& pointCloud);

    int GetNumofPoints() const { return points.size(); }
    int GetEpsilon() const {return eps; }
    const std::vector<CPoint2D>& GetPoints() const { return points; }
    const std::vector<std::vector<int>>& GetObjectSortedID() const { return objectSortedID; }


  private:
    unsigned int minPts;    // Minimum points in a cluster
    float eps;  // Minimum distnce between points

    std::vector<CPoint2D> points;  // Data points
    std::vector<std::vector<int>> objectSortedID; // Sorted Objects' ID
};


#endif // _DBSCAN_H_