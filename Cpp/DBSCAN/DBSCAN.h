#ifndef _DBSCAN_H_
#define _DBSCAN_H_

#include <vector>
#include <algorithm>
#include <math.h>

#include "Geometry.h"


class DBSCAN{
  public:
    DBSCAN(void);    // Original constructor for DBSCAN
    DBSCAN(unsigned int minPts_, float eps_, std::vector<CPoint2D> points_);

    ~DBSCAN() {};   // Original destructor for DBSCAN
    
    void RunDBSCANTask();
    void ExpandCluster(CPoint2D& currentPoint, int *clusterID, std::vector<int> neighborPtsIndex);
    std::vector<int> RegionQuery(CPoint2D currentPoint);       // Calculate neighbor points' indices
    
    std::vector<std::vector<CPoint2D>> ObjectSorted(); 
    inline float CalculateDistance (CPoint2D& currentPoint, CPoint2D& targetPoint);

    int GetNumofPoints() const { return points.size(); }
    int GetEpsilon() const {return eps; }
    const std::vector<CPoint2D>& GetPoints() const { return points; }


  private:
    unsigned int minPts;    // Minimum points in a cluster
    float eps;  // Minimum distnce between points
    std::vector<CPoint2D> points;  // Data points
};


#endif // _DBSCAN_H_