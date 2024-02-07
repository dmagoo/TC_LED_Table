#include "node_geometry.h"
#include <cmath>

float node_geometry::lerp(int a, int b, float t) {
    return static_cast<float>(a) + (static_cast<float>(b) - static_cast<float>(a)) * t;
}

std::vector<float> node_geometry::cube_lerp(const CubeCoordinate &coordA, const CubeCoordinate &coordB, float t) {
    std::vector<float> result;
    result.push_back(lerp(coordA.getQ(), coordB.getQ(), t));
    result.push_back(lerp(coordA.getR(), coordB.getR(), t));
    result.push_back(lerp(coordA.getS(), coordB.getS(), t));
    return result;
}

CubeCoordinate node_geometry::cube_round(float q_float, float r_float, float s_float) {
   int q = static_cast<int>(std::round(q_float));
   int r = static_cast<int>(std::round(r_float));
   int s = static_cast<int>(std::round(s_float));

    float q_diff = std::abs(q - q_float);
    float r_diff = std::abs(r - r_float);
    float s_diff = std::abs(s - s_float);

    if (q_diff > r_diff && q_diff > s_diff) {
        q = -r - s;
    } else if (r_diff > s_diff) {
        r = -q - s;
    } else {
        s = -q - r;
    }

    return CubeCoordinate(q, r, s);
}

int node_geometry::get_cube_distance(CubeCoordinate coordA, CubeCoordinate coordB) {
    CubeCoordinate diff = coordA - coordB;
    return std::max({std::abs(diff.getQ()), std::abs(diff.getR()), std::abs(diff.getS())});
}

std::vector<int> node_geometry::get_node_path(int nodeIdA, int nodeIdB, const ClusterManager &clusterManager) {

    CubeCoordinate coordA = clusterManager.getCubeCoordinate(nodeIdA);
    CubeCoordinate coordB = clusterManager.getCubeCoordinate(nodeIdB);
    int distance = get_cube_distance(coordA, coordB);

    std::vector<int> result;
    result.reserve(distance);

    for (int i = 0; i <= distance; i++) {
        std::vector<float> lerp_values = cube_lerp(coordA, coordB, 1.0 / distance * i);
        CubeCoordinate rounded_values = cube_round(lerp_values[0], lerp_values[1], lerp_values[2]);
        result.push_back(clusterManager.getNodeId(rounded_values));
    }
    return result;
}

std::vector<int> node_geometry::get_node_neighbors(int nodeId, const ClusterManager &clusterManager) {
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
        CubeCoordinate::NorthWest()};

    // Iterate over each direction, compute the neighbor's ID, and add to the result
    for (const auto &direction : directions) {
        int neighborId = clusterManager.getNodeId(start + direction);
        // Include valid and invalid neighbors, assuming -1 indicates an invalid node
        result.push_back(neighborId);
    }

    return result;
}
