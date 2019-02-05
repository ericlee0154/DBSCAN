#include "ObjectClustering.h"

ds::ObjectClustering::ObjectClustering(void): minPts(0), eps(0.0F), bounding_box_type(REC_VCS) { }
ds::ObjectClustering::ObjectClustering(unsigned int minPts_ip, float eps_ip, std::vector<CPoint2D> points_ip, eBoundingBoxType bounding_box_type_ip):
    minPts(minPts_ip),
    eps(eps_ip), 
    points(points_ip),
    objectSortedID (),
    bounding_box_type (bounding_box_type_ip),
    objectBBPts() { }


void ds::ObjectClustering::RunObjClusTask()
{
    unsigned int clusterSize = DBSCAN();

    // Initialize objectSortedID column: clusterSize counts from 1
    objectSortedID.resize(clusterSize);
    // Take sorted outside. Because we may want to track object's ID.
    LabelObjects();

    // Initialize objectBBPts (Bounding Box Points)
    objectBBPts.resize(objectSortedID.size());
    // Draw bounding box / minimum bunding box / boundary 
    ObjectsContour();


}


unsigned int ds::ObjectClustering::DBSCAN()
{
    std::vector<CPoint2D>::iterator iter;
    unsigned int currentClusterID = 1;
    
    // Visit each point in the data set
    for (iter = points.begin(); iter != points.end(); ++iter )
    {
        if (iter->clusterID == 0)
        {
            std::vector<unsigned int> neighborPtsIndex = RegionQuery(*iter);

            if ( neighborPtsIndex.size() < (minPts-1) )  // minus 1 to remove counting center point
                { iter->clusterID = -1; }       // Noise Point
            else
            {
                iter->clusterID = currentClusterID; // Assign current point's clusterID
                ExpandCluster(currentClusterID, neighborPtsIndex);
                currentClusterID = currentClusterID + 1;    // Finish current cluster, move to next cluster
            }
        }
    }   // End of for-loop
    return (currentClusterID -1);
}

void ds::ObjectClustering::ExpandCluster(const int& clusterID, std::vector<unsigned int>& neighborPtsIndex)
{
    std::vector<unsigned int>::iterator iter_clus;

    for (iter_clus = neighborPtsIndex.begin(); iter_clus != neighborPtsIndex.end(); ++iter_clus)
    {
        points.at(*iter_clus).clusterID = clusterID;  // Assign cluster ID into neighbor points
    }

    std::vector<unsigned int>::iterator iter_ext;
    // Assign current point into current cluster
    for (iter_ext = neighborPtsIndex.begin(); iter_ext != neighborPtsIndex.end(); ++iter_ext)
    {
        std::vector<unsigned int> neighborPtsIndex_ext = RegionQuery(points.at(neighborPtsIndex[iter_ext-neighborPtsIndex.begin()]));

        if (neighborPtsIndex_ext.size() >= (minPts-1) )   // minus 1 to remove counting center point
        {
            std::vector<unsigned int>::iterator iter_ext;

            for (iter_ext = neighborPtsIndex_ext.begin(); iter_ext != neighborPtsIndex_ext.end(); ++iter_ext)
            {
                if (points.at(*iter_ext).clusterID == 0 )
                {                    
                    neighborPtsIndex.push_back(*iter_ext);

                    points.at(*iter_ext).clusterID = clusterID;
                }
            }   // Finish union
        }
    }   // Ending of for-loop
};

// May need to track ID. Currently just simple implementation
// If no need to track ID. Put it into DBSCAN without compuatation waste.
// Candidate: void TrackingLabelObjects()
void ds::ObjectClustering::LabelObjects ()
{
    std::vector<CPoint2D>::iterator iter;

    for (iter = points.begin(); iter != points.end(); ++iter )
    {
        if(iter->clusterID != -1)
        {
            objectSortedID[iter->clusterID-1].push_back(iter-points.begin());
        }
        else
        {
            std::vector<unsigned int> noisePoint(1,iter-points.begin());
            objectSortedID.push_back(noisePoint);
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
void ds::ObjectClustering::ObjectsContour ()
{
    switch ( bounding_box_type )
    {
        case REC_VCS:       // Case 0: Draw rectanglar bounding box based on current coordinate system axes
        {
            // May use Template function
            std::vector<std::vector<unsigned int> >::iterator iter_row;
            std::vector<unsigned int>::iterator iter_cln;
            for (iter_row = objectSortedID.begin(); iter_row != objectSortedID.end(); ++iter_row)
            {
                // Initialization
                float x_max = -FLT_MAX_NUM;
                float x_min =  FLT_MAX_NUM;
                float y_max = -FLT_MAX_NUM;
                float y_min =  FLT_MAX_NUM;

                for (iter_cln = (*iter_row).begin(); iter_cln != (*iter_row).end(); ++iter_cln )
                {
                    if (points[*iter_cln].x > x_max)
                        { x_max = points[*iter_cln].x; }

                    if (points[*iter_cln].x < x_min)
                        { x_min = points[*iter_cln].x; }

                    if (points[*iter_cln].y > y_max)
                        { y_max = points[*iter_cln].y; }

                    if (points[*iter_cln].y < y_min)
                        { y_min = points[*iter_cln].y; }
                }

                // ISO
                Point2D upper_left_pt(x_max, y_max);
                Point2D lower_right_pt(x_min, y_min);
                objectBBPts[iter_row-objectSortedID.begin()].push_back(upper_left_pt);
                objectBBPts[iter_row-objectSortedID.begin()].push_back(lower_right_pt);
            }
            break;
        }
        
        case MINIMUM_BOUNDING_REC:      // Case 1: Draw minimum rectangular bounding box of each object
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

std::vector<unsigned int> ds::ObjectClustering::RegionQuery (CPoint2D currentPoint)
{
    std::vector<unsigned int> neighborPtsIndex;
    std::vector<CPoint2D>::iterator iter;

    for (iter = points.begin(); iter != points.end(); ++iter)
    {
        // Not count current point
        if ( ( (currentPoint.x) != (iter->x) ) && ( (currentPoint.y) != (iter->y) ) )
        {
            if (CalculateDistance(currentPoint, *iter) <= (eps*eps) )
            {
                neighborPtsIndex.push_back(iter-points.begin());    // Store index
            }
        }
    }
    return neighborPtsIndex;
};

inline float ds::ObjectClustering::CalculateDistance(const CPoint2D& currentPoint, const CPoint2D& targetPoint)
{
    return pow(currentPoint.x - targetPoint.x,2) + pow(currentPoint.y - targetPoint.y,2);
};

inline void ds::ObjectClustering::CalculateCenterPt(const CPoint2D& currentPoint, Point2D& centerPoint, unsigned int& weight)
{
    centerPoint.x = (currentPoint.x + (centerPoint.x * weight) ) / (weight + float(1));
    centerPoint.y = (currentPoint.y + (centerPoint.y * weight) ) / (weight + float(1));
    weight = weight + 1;
};