#include "ClusterManager.h"

ClusterManager::ClusterManager(const std::vector<ClusterConfig> &clusterConfigs) {
    for (const auto &clusterConfig : clusterConfigs) {
        clusters.emplace_back(clusterConfig.clusterId, clusterConfig.nodeConfigs);
        clusterIdToIndexMap[clusterConfig.clusterId] = clusters.size() - 1;
        for (const auto &nodeConfig : clusterConfig.nodeConfigs) {
            ringCoordinateToNodeId[nodeConfig.getRingCoordinates()] = nodeConfig.getId();
            cubeCoordinateToNodeId[nodeConfig.getCubeCoordinates()] = nodeConfig.getId();
            cartesian2dCoordinateToNodeId[nodeConfig.getCartesian2dCoordinates()] = nodeConfig.getId();

            nodeIdToRingCoordinate[nodeConfig.getId()] = nodeConfig.getRingCoordinates();
            nodeIdToCubeCoordinate[nodeConfig.getId()] = nodeConfig.getCubeCoordinates();
            nodeIdToCartesian2dCoordinate[nodeConfig.getId()] = nodeConfig.getCartesian2dCoordinates();
        }
    }
}

const Cluster *ClusterManager::getClusterById(int clusterId) const {
    auto it = clusterIdToIndexMap.find(clusterId);
    if (it != clusterIdToIndexMap.end()) {
        return &clusters[it->second];
    }
    return nullptr; // Or handle the error as appropriate
}


int ClusterManager::getNodeId(const RingCoordinate &coords) const {
    auto it = ringCoordinateToNodeId.find(coords);
    if (it != ringCoordinateToNodeId.end()) {
        return it->second;
    }
    return -1; // Handle not found
}

int ClusterManager::getNodeId(const CubeCoordinate &coords) const {
    auto it = cubeCoordinateToNodeId.find(coords);
    if (it != cubeCoordinateToNodeId.end()) {
        return it->second;
    }
    return -1; // Handle not found
}

int ClusterManager::getNodeId(const Cartesian2dCoordinate &coords) const {
    auto it = cartesian2dCoordinateToNodeId.find(coords);
    if (it != cartesian2dCoordinateToNodeId.end()) {
        return it->second;
    }
    return -1; // Handle not found
}

RingCoordinate ClusterManager::getRingCoordinate(int nodeId) const {
    auto it = nodeIdToRingCoordinate.find(nodeId);
    if (it != nodeIdToRingCoordinate.end()) {
        return it->second;
    }
    return RingCoordinate();
}

CubeCoordinate ClusterManager::getCubeCoordinate(int nodeId) const {
    auto it = nodeIdToCubeCoordinate.find(nodeId);
    if (it != nodeIdToCubeCoordinate.end()) {
        return it->second;
    }
    return CubeCoordinate();
}

Cartesian2dCoordinate ClusterManager::getCartesian2dCoordinate(int nodeId) const {
    auto it = nodeIdToCartesian2dCoordinate.find(nodeId);
    if (it != nodeIdToCartesian2dCoordinate.end()) {
        return it->second;
    }
    return Cartesian2dCoordinate();
}
