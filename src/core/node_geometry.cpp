#include "node_geometry.h"
#include <iostream>

std::vector<int> node_geometry::getNodePath(int nodeIdA, int nodeIdB, const ClusterManager& clusterManager) {
    return {0};
}

std::vector<int> node_geometry::getNodeNeighbors(int nodeId, const ClusterManager& clusterManager) {
    CubeCoordinate start = clusterManager.getCubeCoordinate(nodeId);
    std::vector<int> result;
    result.reserve(6);

    // List of directional transformations
    std::vector<CubeCoordinate> directions = {
        CubeCoordinate::North(),
        CubeCoordinate::NorthEast(),
        CubeCoordinate::SouthEast(),
        CubeCoordinate::South(),
        CubeCoordinate::SouthWest(),
        CubeCoordinate::NorthWest()
    };

    // Iterate over each direction, compute the neighbor's ID, and add to the result
    for (const auto& direction : directions) {
        int neighborId = clusterManager.getNodeId(start + direction);
        // Include valid and invalid neighbors, assuming -1 indicates an invalid node
        result.push_back(neighborId);
    }

    return result;
}
