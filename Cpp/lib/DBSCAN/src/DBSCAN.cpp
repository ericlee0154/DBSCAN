#include "DBSCAN.h"
namespace geo{

  DBSCAN::DBSCAN(void): min_pts(0U), eps(0.0F) { }
  DBSCAN::DBSCAN(const unsigned int min_pts_ip, const double eps_ip, const std::vector<CPoint2D> points_ip):
      min_pts(min_pts_ip),
      eps(eps_ip),
      points(points_ip)
      {
        std::vector<CPoint2D>::iterator iter;
        unsigned int current_cluster_id = 0;
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
                    ++current_cluster_id;    // Finish current cluster, move to next cluster
                    iter->SetClusterID(current_cluster_id); // Assign current point's clusterID
                    ExpandCluster(current_cluster_id, neighbor_pts_id);
                }
            }
            neighbor_pts_id.clear();
        }   // End of for-loop

        neighbor_pts_id.shrink_to_fit();
      }





  void DBSCAN::ExpandCluster(const int& cluster_id, std::vector<unsigned int>& neighbor_pts_id)
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
          RegionQuery(neighbor_pts_id_ext, points.at(neighbor_pts_id[index]), neighbor_pts_id[index]);

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


  void DBSCAN::RegionQuery(std::vector<unsigned int>& neighbor_pts_id, const CPoint2D& current_point, const unsigned int current_point_index)
  {
      std::vector<CPoint2D>::iterator iter;

      for (iter = points.begin(); iter != points.end(); ++iter)
      {
          // Doesn't count current point
          if  (  current_point_index != (iter-points.begin()) && (current_point != (*iter) ) )
          {
              neighbor_pts_id.push_back( iter-points.begin() );    // Store index
          }
      }
  };

} // namespace geo
