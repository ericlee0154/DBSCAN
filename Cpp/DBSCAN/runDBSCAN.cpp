#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <time.h>

#include "DBSCAN.h"

// #define MINIMUM_POINTS 4         // Define minimum points: 4
// #define EPSILON        0.75      // Define minimum distance: 0.2


std::vector<int> readData(std::vector<CPoint2D>& rawPoints, float &eps, unsigned int &minPts)
{
    // point cloud
    FILE *stream;
    stream = fopen ("../../Python36/dataset_py.txt","ra");
    
    unsigned int num_points;
    std::vector<int> clusterGT;
    int cluster, i = 0;
    fscanf(stream, "%u \n%f \n%d \n", &num_points, &eps, &minPts);

    CPoint2D *p = (CPoint2D *)std::calloc(num_points, sizeof(CPoint2D));
    p->clusterID = 0;
    p->visited = false;

    while (i < num_points)
    {
        float x, y;
        fscanf(stream, "%f, %f, %d, \n", &(p->x), &(p->y), &cluster);

        if (cluster != -1)
        {
            cluster = cluster + 1;
        }

        clusterGT.push_back(cluster);
        rawPoints.push_back(*p);
        ++i;
    }
    free(p);
    fclose(stream);

    return clusterGT;
}

void printResults(const std::vector<CPoint2D>& Points, int num_points, std::vector<int> clusterGT)
{
    int i = 0;
    int cluster_check = 0;
    int numofCluster = 0;
    int noisePoints = 0;
    int index;
    printf("Number of points: %u\n"
        " x     y     cluster_id\n"
        "-----------------------------\n"
        , num_points);
    while (i < num_points)
    {
        printf("%5.2lf %5.2lf: %d\n",
                Points[i].x,
                Points[i].y,
                Points[i].clusterID);
        
        if (Points[i].clusterID != clusterGT[i])
        {
            cluster_check = cluster_check + 1;
            index = i;
        }
        if (Points[i].clusterID > numofCluster)
            numofCluster = Points[i].clusterID;

        if (Points[i].clusterID == -1)
            ++noisePoints;

        ++i;
    }
    std::cout << "Number of Clusters: " << numofCluster << std::endl;
    std::cout << "Number of Noise Points: " << noisePoints << std::endl;
    std::cout << "Cluster Check: " << cluster_check << ".  Zero means no difference. Total Points: " << num_points << std::endl;
}

int main()
{    
    std::vector<CPoint2D> rawPoints;

    float eps;
    unsigned int minPts;
    // read point data
    std::vector<int> clusterGT = readData(rawPoints, eps, minPts);

    // constructor
    DBSCAN ds(minPts, eps, rawPoints);
    
    double start, end;
    start = clock();
    // main loop
    ds.runDBSCANTask();
    end = clock();

    // result of DBSCAN algorithm
    printResults(ds.getPoints(), ds.getNumofPoints(), clusterGT);

    std::cout << "Computation time " << (end - start)/CLOCKS_PER_SEC << " s" << std::endl;

    return 0;
}