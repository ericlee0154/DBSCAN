#include "ObjectCluster.h"

ObjectCluster::ObjectCluster(void): min_pts(0U), eps(0.0F) { }
ObjectCluster::ObjectCluster(const unsigned int min_pts_ip, const float eps_ip, const std::vector<CPoint2D> points_ip):
    min_pts(min_pts_ip),
    eps(eps_ip),
    points(points_ip)
    {
        unsigned int clusterSize = DBSCAN();

        // Initialize object_sorted_id column: clusterSize counts from 1
        object_sorted_id.resize(clusterSize);
        // Take sorted outside. Because we may want to track object's ID.
        LabelObjects();
        // Initialize objectBBPts (Bounding Box Points)
        objectBBPts.resize(object_sorted_id.size());
        // Draw bounding box / minimum bunding box / boundary 
        DrawObjectsContour();
    }


unsigned int ObjectCluster::DBSCAN()
{
    std::vector<CPoint2D>::iterator iter;
    unsigned int currentClusterID = 0;
    std::vector<unsigned int>neighbor_pts_id;
    neighbor_pts_id.reserve(points.size());
    
    // Visit each point in the data set
    for (iter = points.begin(); iter != points.end(); ++iter )
    {
        if (iter->GetClusterID() == 0)
        {
            RegionQuery(neighbor_pts_id, *iter, iter-points.begin());

            if ( neighbor_pts_id.size() < (min_pts-1) )  // minus 1 to remove counting center point
                { iter->SetClusterID(-1); }       // Noise Point
            else
            {
                ++currentClusterID;    // Finish current cluster, move to next cluster
                iter->SetClusterID(currentClusterID); // Assign current point's clusterID
                ExpandCluster(currentClusterID, neighbor_pts_id);
            }
        }
        neighbor_pts_id.clear();
    }   // End of for-loop

    neighbor_pts_id.shrink_to_fit();
    return currentClusterID;
}

void ObjectCluster::ExpandCluster(const int& cluster_id, std::vector<unsigned int>& neighbor_pts_id)
{
    std::vector<unsigned int>::iterator iter_clus;

    for (iter_clus = neighbor_pts_id.begin(); iter_clus != neighbor_pts_id.end(); ++iter_clus)
    {
        points.at( *iter_clus ).SetClusterID(cluster_id);  // Assign cluster ID into neighbor points
    }

    std::vector<unsigned int> neighbor_pts_id_ext;
    neighbor_pts_id_ext.reserve(points.size());
    // Assign current point into current cluster
    for (int index = 0; index < neighbor_pts_id.size(); ++index)
    {
        RegionQuery( neighbor_pts_id_ext, points.at(neighbor_pts_id[index]), neighbor_pts_id[index]);

        if (neighbor_pts_id_ext.size() >= (min_pts-1) )   // minus 1 to remove counting center point
        {
            std::vector<unsigned int>::iterator iter_ext;

            for (iter_ext = neighbor_pts_id_ext.begin(); iter_ext != neighbor_pts_id_ext.end(); ++iter_ext)
            {
                if ( points[*iter_ext].GetClusterID() == 0 )
                {
                    neighbor_pts_id.push_back(*iter_ext);
                    points[*iter_ext].SetClusterID(cluster_id);
                }
            }   // Finish union
        }
        neighbor_pts_id_ext.clear();
    }   // Ending of for-loop
    neighbor_pts_id_ext.shrink_to_fit();
};

// May need to track ID. Currently just simple implementation
// If no need to track ID. Put it into DBSCAN without compuatation waste.
// Candidate: void TrackingLabelObjects()
void ObjectCluster::LabelObjects ()
{
    std::vector<CPoint2D>::iterator iter;

    for (iter = points.begin(); iter != points.end(); ++iter )
    {
        if(iter->GetClusterID() != -1)
        {
            object_sorted_id[iter->GetClusterID()-1].push_back(iter-points.begin());
        }
        else
        {
            std::vector<unsigned int> noisePoint(1,iter-points.begin());
            object_sorted_id.push_back(noisePoint);
        }
    }
}

void ObjectCluster::DrawObjectsContour ()
{
    // May use Template function
    std::vector<std::vector<unsigned int> >::iterator iter_row;
    std::vector<unsigned int>::iterator iter_cln;
    for (iter_row = object_sorted_id.begin(); iter_row != object_sorted_id.end(); ++iter_row)
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
        objectBBPts[iter_row-object_sorted_id.begin()].push_back(upper_left_pt);
        objectBBPts[iter_row-object_sorted_id.begin()].push_back(lower_right_pt);
    }
};

void ObjectCluster::RegionQuery(std::vector<unsigned int>& neighbor_pts_id, const CPoint2D& currentPoint, const unsigned int current_point_index)
{
    std::vector<CPoint2D>::iterator iter;

    for (iter = points.begin(); iter != points.end(); ++iter)
    {
        float tolerance = 1e-03F;
        // Not count current point
        if  (  current_point_index != (iter-points.begin()) && CalculateDistance(currentPoint, *iter) <= (eps*eps) )
        {
            neighbor_pts_id.push_back( iter-points.begin() );    // Store index
        }
    }
};

inline float ObjectCluster::CalculateDistance(const CPoint2D& currentPoint, const CPoint2D& targetPoint)
{
    return ( (currentPoint.GetX() - targetPoint.GetX())*(currentPoint.GetX() - targetPoint.GetX()) ) + ( (currentPoint.GetY() - targetPoint.GetY())*(currentPoint.GetY() - targetPoint.GetY()) );
};

inline void ObjectCluster::CalculateCenterPt(const CPoint2D& currentPoint, Point2D& centerPoint, float& weight)
{
    centerPoint.SetX(   ((currentPoint.GetX() + (centerPoint.GetX() * weight) ) / (weight + 1.0F)) );
    centerPoint.SetY(   ((currentPoint.GetY() + (centerPoint.GetY() * weight) ) / (weight + 1.0F)) );
    ++weight;
};