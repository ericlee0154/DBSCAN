#ifndef _OBJECTCLUSTERING_H_
#define _OBJECTCLUSTER_H_

#include <vector>
#include <algorithm>
#include <math.h>

#include "Geometry.h"

#define FLT_MAX_NUM __FLT_MAX__

namespace ds {    // Namespcae : Drivable Space

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
    void ExpandCluster(const int& clusterID, std::vector<unsigned int>& neighborPtsIndex);
    std::vector<unsigned int> RegionQuery(CPoint2D currentPoint);       // Calculate neighbor points' indices
    // potential functions
    void LabelObjects();
    void ObjectsContour();

    inline float CalculateDistance (const CPoint2D& currentPoint, const CPoint2D& targetPoint);
    inline void CalculateCenterPt (const CPoint2D& currentPoint, Point2D& centerPoint, unsigned int& weight);

    // Call functions
    unsigned int GetNumofPoints() const { return points.size(); }
    float GetEpsilon() const { return eps; }
    const eBoundingBoxType GetBoundingBoxType() const { return bounding_box_type; }
    const std::vector<CPoint2D>& GetPoints() const { return points; }
    const std::vector<std::vector<unsigned int> >& GetObjectSortedID() const { return objectSortedID; }
    const std::vector<std::vector<Point2D> >& GetObjectBoundingBoxPoints() const { return objectBBPts; }


  private:
    unsigned int minPts;    // Minimum points in a cluster
    float eps;  // Minimum distance between points

    eBoundingBoxType bounding_box_type;   // Bounding box type

    std::vector<CPoint2D> points;  // Data points
    std::vector<std::vector<unsigned int> > objectSortedID; // Sorted Objects' ID
    std::vector<std::vector<Point2D> > objectBBPts;   // Return bounding boxes' points
};

}

#endif // _OBJECTCLUSTERING_H_