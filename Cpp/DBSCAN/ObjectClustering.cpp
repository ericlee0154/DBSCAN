#include "ObjectClustering.h"

ObjectClustering::ObjectClustering(void) { }
ObjectClustering::ObjectClustering(unsigned int minPts_, float eps_, std::vector<CPoint2D> points_, eBoundingBoxType bounding_box_type_):
    minPts(minPts_),
    eps(eps_), 
    points(points_),
    objectSortedID (),
    bounding_box_type (bounding_box_type_),
    objectBBPts() { }


void ObjectClustering::RunObjClusTask()
{
    int clusterSize = DBSCAN();

    // Initialize objectSortedID column: clusterSize counts from 1
    objectSortedID.resize(clusterSize);
    // Take sorted outside. Because we may want to track object's ID.
    LabelObjects();
    // Draw bounding box / minimum bunding box / boundary point
    ObjectsContour();


}



int ObjectClustering::DBSCAN()
{
    std::vector<CPoint2D>::iterator iter;
    int currentClusterID = 1;
    
    // Visit each point in the data set
    for (iter = points.begin(); iter != points.end(); ++iter )
    {
        if (iter->clusterID == 0)
        {
            std::vector<int> objectBDPts;    // Object Boundary Points
            std::vector<int> neighborPtsIndex = RegionQuery(*iter);

            if ( neighborPtsIndex.size() < minPts - 1)  // minus 1 to remove counting center point
                iter->clusterID = -1;       // Noise Point
            else
            {
                iter->clusterID = currentClusterID; // Assign current point's clusterID
                ExpandCluster(&currentClusterID, neighborPtsIndex, objectBDPts);
                currentClusterID = currentClusterID + 1;    // Finish current cluster, move to next cluster
            }
        }
    }   // End of for-loop
    return currentClusterID;
}

void ObjectClustering::ExpandCluster(int *clusterID, std::vector<int>& neighborPtsIndex, std::vector<int>& objectBDPts)
{
    std::vector<int>::iterator iter_clus;

    for (iter_clus = neighborPtsIndex.begin(); iter_clus != neighborPtsIndex.end(); ++iter_clus)
    {
        points.at(*iter_clus).clusterID = *clusterID;  // Assign cluster ID into neighbor points
    }

    // Assign current point into current cluster
    for (int i = 0; i < neighborPtsIndex.size(); i++)
    {
        std::vector<int> neighborPtsIndex_ext = RegionQuery(points.at(neighborPtsIndex[i]));

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

// May need to track ID. Currently just simple implementation
// If no need to track ID. Put it into DBSCAN without compuatation waste.
// Candidate: void TrackingLabelObjects()
void ObjectClustering::LabelObjects ()
{
    std::vector<CPoint2D>::iterator iter;
    int index = 0;

    for (iter = points.begin(); iter != points.end(); ++iter )
    {
        if(iter->clusterID != -1)
        {
            objectSortedID[iter->clusterID-1].push_back(index);
            ++index;
        }
        else
        {
            std::vector<int> noisePoint(1,index);
            objectSortedID.push_back(noisePoint);
            ++index;
        }
    }
}


// Currently brute-force; waiting for further design
/*
template <typename T>
int main(){

   int a = 0;
   ObjectClustering::ObjectsContour<double>())
}
*/
void ObjectClustering::ObjectsContour ()
{
    switch ( bounding_box_type )
    {
        case REC_VCS:
        {
            // May use Template function
            float pInf = FLT_MAX;   // Assign float maximum number
            float nInf = -FLT_MAX;
            int obj_index;

            std::vector<std::vector<int> >::iterator iter_row;
            std::vector<int>::iterator iter_cln;
            for (iter_row = objectSortedID.begin(); iter_row != objectSortedID.end(); ++iter_row)
            {
                float x_max = nInf;
                float x_min = pInf;
                float y_max = nInf;
                float y_min = pInf;

                for (iter_cln = (*iter_row).begin(); iter_cln != (*iter_row).end(); ++iter_cln )
                {
                    if (points[*iter_cln].x > x_max)
                        x_max = points[*iter_cln].x;

                    if (points[*iter_cln].x < x_min)
                        x_min = points[*iter_cln].x;

                    if (points[*iter_cln].y > y_max)
                        y_max = points[*iter_cln].y;

                    if (points[*iter_cln].y > y_min)
                        y_min = points[*iter_cln].y;
                }
                // ISO
                Point2D upper_left_pt(x_max, y_max);
                Point2D lower_right_pt(x_min, y_min);
                objectBBPts[obj_index].push_back(upper_left_pt);
                objectBBPts[obj_index].push_back(lower_right_pt);

                ++obj_index;
            }
            break;
        }
        
        case MINIMUM_BOUNDING_REC:
        {
            break;
        }
        case BOUNDARY_PTS:
        {
            break;
        }
        default:
            break;
    }
};

std::vector<int> ObjectClustering::RegionQuery (CPoint2D currentPoint)
{
    std::vector<int> neighborPtsIndex;
    std::vector<CPoint2D>::iterator iter;
    int index = 0;

    for (iter = points.begin(); iter != points.end(); ++iter)
    {
        // Not count current point
        if (currentPoint.x != iter->x && currentPoint.y != iter->y)
        {
            if (CalculateDistance(currentPoint, *iter) <= eps*eps)
            {
                neighborPtsIndex.push_back(index);
            }
        }
        index = index + 1;
    }
    return neighborPtsIndex;
};

inline float ObjectClustering::CalculateDistance(CPoint2D& currentPoint, CPoint2D& targetPoint)
{
    return pow(currentPoint.x - targetPoint.x,2) + pow(currentPoint.y - targetPoint.y,2);
};

inline void ObjectClustering::CalculateCenterPt(CPoint2D& currentPoint, Point2D& centerPoint, unsigned int& weight)
{
    centerPoint.x = (currentPoint.x + centerPoint.x * weight) / (weight + float(1));
    centerPoint.y = (currentPoint.y + centerPoint.y * weight) / (weight + float(1));
    weight = weight + 1;
};
