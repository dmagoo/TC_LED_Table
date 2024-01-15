// ClusterManager.h
#ifndef CLUSTERMANAGER_H
#define CLUSTERMANAGER_H

#include "ClusterConfig.h"
#include "Cluster.h"
#include "coordinates/RingCoordinate.h"
#include "coordinates/CubeCoordinate.h"
#include "coordinates/Cartesian2dCoordinate.h"
#include <vector>
#include <unordered_map>

class ClusterManager {
private:
    std::vector<Cluster> clusters; // Vector to maintain the order of clusters

    // Mappings for efficient lookup and reverse lookup
    std::unordered_map<int, size_t> clusterIdToIndexMap;
    std::unordered_map<int, int> nodeIdToClusterIdMap;

    std::unordered_map<RingCoordinate, int> ringCoordinateToNodeId;
    std::unordered_map<CubeCoordinate, int> cubeCoordinateToNodeId;
    std::unordered_map<Cartesian2dCoordinate, int> cartesian2dCoordinateToNodeId;

    std::unordered_map<int, RingCoordinate> nodeIdToRingCoordinate;
    std::unordered_map<int, CubeCoordinate> nodeIdToCubeCoordinate;
    std::unordered_map<int, Cartesian2dCoordinate> nodeIdToCartesian2dCoordinate;

public:
    ClusterManager(const std::vector<ClusterConfig> &clusterConfigs);
    const Cluster* getClusterById(int clusterId) const;

    int getNodeId(const RingCoordinate &coords) const;
    int getNodeId(const CubeCoordinate &coords) const;
    int getNodeId(const Cartesian2dCoordinate &coords) const;

    RingCoordinate getRingCoordinate(int nodeId) const;
    CubeCoordinate getCubeCoordinate(int nodeId) const;
    Cartesian2dCoordinate getCartesian2dCoordinate(int nodeId) const;
};

#endif // CLUSTERMANAGER_H
