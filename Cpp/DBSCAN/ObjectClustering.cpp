#include "ObjectClustering.h"

ds::ObjectClustering::ObjectClustering(void): minPts(0U), eps(0.0F), bounding_box_type(REC_VCS) { }
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
    objectSortedID.resize(static_cast<unsigned int64_t>(clusterSize));
    // Take sorted outside. Because we may want to track object's ID.
    LabelObjects();

    // Initialize objectBBPts (Bounding Box Points)
    objectBBPts.resize(objectSortedID.size());
    // Draw bounding box / minimum bunding box / boundary 
    DrawObjectsContour();


}


unsigned int ds::ObjectClustering::DBSCAN()
{
    std::vector<CPoint2D>::iterator iter;
    unsigned int currentClusterID = 1U;
    
    // Visit each point in the data set
    for (iter = points.begin(); iter != points.end(); ++iter )
    {
        if (iter->GetClusterID() == 0)
        {
            std::vector<unsigned int> neighborPtsIndex = RegionQuery(*iter);

            if ( static_cast<unsigned int32_t>(neighborPtsIndex.size()) < (minPts-1U) )  // minus 1 to remove counting center point
                { iter->SetClusterID(-1); }       // Noise Point
            else
            {
                iter->SetClusterID(static_cast<signed int>(currentClusterID)); // Assign current point's clusterID
                ExpandCluster(static_cast<signed int>(currentClusterID), neighborPtsIndex);
                ++currentClusterID;    // Finish current cluster, move to next cluster
            }
        }
    }   // End of for-loop
    return (currentClusterID -1U);
}

void ds::ObjectClustering::ExpandCluster(const int& clusterID, std::vector<unsigned int>& neighborPtsIndex)
{
    std::vector<unsigned int>::iterator iter_clus;

    for (iter_clus = neighborPtsIndex.begin(); iter_clus != neighborPtsIndex.end(); ++iter_clus)
    {
        points.at( static_cast<unsigned int64_t>(*iter_clus) ).SetClusterID(clusterID);  // Assign cluster ID into neighbor points
    }

    // Assign current point into current cluster
    for (int index = 0; static_cast<unsigned int64_t>(index) < neighborPtsIndex.size(); ++index)
    {
        std::vector<unsigned int> neighborPtsIndex_ext = RegionQuery(points.at( static_cast<unsigned int64_t>(neighborPtsIndex[index]) ));

        if (static_cast<unsigned int32_t>(neighborPtsIndex_ext.size()) >= (minPts-1U) )   // minus 1 to remove counting center point
        {
            std::vector<unsigned int>::iterator iter_ext;

            for (iter_ext = neighborPtsIndex_ext.begin(); iter_ext != neighborPtsIndex_ext.end(); ++iter_ext)
            {
                if (points.at( static_cast<unsigned int64_t>(*iter_ext) ).GetClusterID() == 0 )
                {
                    neighborPtsIndex.push_back(*iter_ext);
                    points.at( static_cast<unsigned int64_t>(*iter_ext) ).SetClusterID(clusterID);
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
        if(iter->GetClusterID() != -1)
        {
            unsigned int clusAbsID = static_cast<unsigned int>(std::abs(iter->GetClusterID()-1));
            objectSortedID[clusAbsID].push_back(static_cast<unsigned int>(iter-points.begin()) );
        }
        else
        {
            std::vector<unsigned int> noisePoint(1U,static_cast<unsigned int>(iter-points.begin()) );
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
void ds::ObjectClustering::DrawObjectsContour ()
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
                    if (points[*iter_cln].GetX() > x_max)
                        { x_max = points[*iter_cln].GetX(); }

                    if (points[*iter_cln].GetX() < x_min)
                        { x_min = points[*iter_cln].GetX(); }

                    if (points[*iter_cln].GetY() > y_max)
                        { y_max = points[*iter_cln].GetY(); }

                    if (points[*iter_cln].GetY() < y_min)
                        { y_min = points[*iter_cln].GetY(); }
                }

                // ISO
                Point2D upper_left_pt(x_max, y_max);
                Point2D lower_right_pt(x_min, y_min);
                objectBBPts[iter_row-objectSortedID.begin()].push_back(upper_left_pt);
                objectBBPts[iter_row-objectSortedID.begin()].push_back(lower_right_pt);
            }
            break;
        }
        
        case MINIMUM_BOUNDING_REC:      // Case 1: Draw minimum rectangule bounding box of each object
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

std::vector<unsigned int> ds::ObjectClustering::RegionQuery (const CPoint2D& currentPoint)
{
    std::vector<unsigned int> neighborPtsIndex;
    std::vector<CPoint2D>::iterator iter;

    for (iter = points.begin(); iter != points.end(); ++iter)
    {
        float tolerance = 1e-03F;
        // Not count current point
        if(  (  ( (currentPoint.GetX()-iter->GetX())>tolerance) || ( (currentPoint.GetX()-iter->GetX())<-tolerance)  )  &&
             (  ( (currentPoint.GetY()-iter->GetY())>tolerance) || ( (currentPoint.GetY()-iter->GetY())<-tolerance)  )  )
        {
            if (CalculateDistance(currentPoint, *iter) <= (eps*eps) )
            {
                neighborPtsIndex.push_back( static_cast<unsigned int>(iter-points.begin()) );    // Store index
            }
        }
    }
    return neighborPtsIndex;
};

inline float ds::ObjectClustering::CalculateDistance(const CPoint2D& currentPoint, const CPoint2D& targetPoint)
{
    return ( (currentPoint.GetX() - targetPoint.GetX())*(currentPoint.GetX() - targetPoint.GetX()) ) + ( (currentPoint.GetY() - targetPoint.GetY())*(currentPoint.GetY() - targetPoint.GetY()) );
};

inline void ds::ObjectClustering::CalculateCenterPt(const CPoint2D& currentPoint, Point2D& centerPoint, float& weight)
{
    centerPoint.SetX(   ((currentPoint.GetX() + (centerPoint.GetX() * weight) ) / (weight + 1.0F)) );
    centerPoint.SetY(   ((currentPoint.GetY() + (centerPoint.GetY() * weight) ) / (weight + 1.0F)) );
    ++weight;
};