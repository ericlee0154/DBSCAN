#ifndef _OBJECTCLUSTERING_H_
#define _OBJECTCLUSTER_H_

#include <vector>
#include <algorithm>
#include <math.h>

#include "Geometry.h"


class ObjectClustering{
  public:
    enum eBoundingBoxType
    {
      REC_VCS = 0,                // Maximum and minimum points to form bounding box
      MINIMUM_BOUNDING_REC,       // Minimum bounding rectangle
      BOUNDARY_PTS                // Boundary points of objects

    };  // The enum indicate the bounding bo type 
  public:
    ObjectClustering(void);    // Original constructor for DBSCAN
    ObjectClustering(unsigned int minPts_ip, float eps_ip, std::vector<CPoint2D> points_ip, eBoundingBoxType bounding_box_type_ip);

    ~ObjectClustering() {};   // Original destructor for DBSCAN
    

    void RunObjClusTask();
    int DBSCAN();
    void ExpandCluster(int *clusterID, std::vector<int>& neighborPtsIndex, std::vector<int>& objectBDPtsID);
    std::vector<int> RegionQuery(CPoint2D currentPoint);       // Calculate neighbor points' indices
    // potential functions
    void LabelObjects(std::vector<std::vector<int> >& objectSortedID);
    void ObjectsContour();

    inline float CalculateDistance (CPoint2D& currentPoint, CPoint2D& targetPoint);
    inline void CalculateCenterPt (CPoint2D& currentPoint, Point2D& centerPoint, unsigned int& weight);

    int GetNumofPoints() const { return points.size(); }
    int GetEpsilon() const {return eps; }
    const eBoundingBoxType GetBoundingBoxType() const {return bounding_box_type; }
    const std::vector<CPoint2D>& GetPoints() const { return points; }
    const std::vector<std::vector<int> >& GetObjectSortedID() const { return objectSortedID; }


  private:
    unsigned int minPts;    // Minimum points in a cluster
    float eps;  // Minimum distnce between points

    eBoundingBoxType bounding_box_type;   // Bounding box type

    std::vector<CPoint2D> points;  // Data points
    std::vector<std::vector<int> > objectSortedID; // Sorted Objects' ID
};


#endif // _DBSCAN_H_