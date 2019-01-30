#include "DBSCAN.h"

DBSCAN::DBSCAN(void) {}
DBSCAN::DBSCAN(unsigned int minPts_, float eps_, std::vector<CPoint2D> points_ ): minPts(minPts_), eps(eps_), points(points_) { }

void DBSCAN::runDBSCANTask()
{
    std::vector<CPoint2D>::iterator iter;
    int currentClusterID = 1;
    // Visit each point in the data set
    for (iter = points.begin(); iter != points.end(); ++iter )
    {
        if (iter->clusterID == 0)
        {
            std::vector<int> neighborPtsIndex = regionQuery(*iter);

            if ( neighborPtsIndex.size() < minPts - 1)  // minus 1 to remove counting center point
                iter->clusterID = -1;       // Noise Point
            else
            {
                iter->clusterID = currentClusterID; // Assign current point's clusterID
                expandCluster(*iter, &currentClusterID, neighborPtsIndex);
                currentClusterID = currentClusterID + 1;    // Finish current cluster, move to next cluster
            }
        }
    }   // End of for-loop
}

void DBSCAN::expandCluster(CPoint2D& currentPoint, int *clusterID, std::vector<int> neighborPtsIndex)
{
    std::vector<int>::iterator iter_clus;

    for (iter_clus = neighborPtsIndex.begin(); iter_clus != neighborPtsIndex.end(); ++iter_clus)
    {
        points.at(*iter_clus).clusterID = *clusterID;  // Assign cluster ID into neighbor points
    }

    // Assign current point into current cluster
    for (int i = 0; i < neighborPtsIndex.size(); i++)
    {
        std::vector<int> neighborPtsIndex_ext = regionQuery(points.at(neighborPtsIndex[i]));

        if (neighborPtsIndex_ext.size() >= minPts -1)   // minus 1 to remove counting center point
        {
            std::vector<int>::iterator iter_ext;

            for (iter_ext = neighborPtsIndex_ext.begin(); iter_ext != neighborPtsIndex_ext.end(); ++iter_ext)
            {
                if (points.at(*iter_ext).clusterID == 0 )
                {                    
                    neighborPtsIndex.push_back(*iter_ext);

                    points.at(*iter_ext).clusterID = *clusterID;
                }
            }   // Finish union
        }
    }   // Ending of for-loop
};

std::vector<int> DBSCAN::regionQuery (CPoint2D currentPoint)
{
    std::vector<int> neighborPtsIndex;
    std::vector<CPoint2D>::iterator iter;
    int index = 0;

    for (iter = points.begin(); iter != points.end(); ++iter)
    {
        // Not count current point
        if (currentPoint.x != iter->x && currentPoint.y != iter->y)
        {
            if (calculateDistance(currentPoint, *iter) <= eps*eps)
            {
                neighborPtsIndex.push_back(index);
            }
        }
        index = index + 1;
    }
    return neighborPtsIndex;
};

inline float DBSCAN::calculateDistance(CPoint2D& currentPoint, CPoint2D& targetPoint)
{
    return pow(currentPoint.x - targetPoint.x,2) + pow(currentPoint.y - targetPoint.y,2);
};