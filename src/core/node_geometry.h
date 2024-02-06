#ifndef NODE_GEOMETRY_H
#define NODE_GEOMETRY_H

#include <vector>

#include "ClusterManager.h"
#include "coordinates/CubeCoordinate.h"

//N
//NE
//SE
//S
//SW
//NW


namespace node_geometry {
    std::vector<int> getNodePath(int nodeIdA, int nodeIdB, const ClusterManager& clusterManager);
    std::vector<int> getNodeNeighbors(int nodeId, const ClusterManager& clusterManager);
 }

 #endif // NODE_GEOMETRY_H