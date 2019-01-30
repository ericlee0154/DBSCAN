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
    
    void runDBSCANTask();
    void expandCluster(CPoint2D& currentPoint, int *clusterID, std::vector<int> neighborPtsIndex);
    std::vector<int> regionQuery(CPoint2D currentPoint);       // Calculate neighbor points' indices
    
    inline float calculateDistance (CPoint2D& currentPoint, CPoint2D& targetPoint);

    int getNumofPoints() const { return points.size(); }
    int getEpsilon() const {return eps; }
    const std::vector<CPoint2D>& getPoints() const { return points; }


  private:
    unsigned int minPts;    // Minimum points in a cluster
    float eps;  // Minimum distnce between points
    std::vector<CPoint2D> points;  // Data points
};


#endif // _DBSCAN_H_