#include "node_geometry.h"
#include <cmath>
#include <iostream>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// If the system changes such that a different number of pixels per node
// is supported, node->getPixelCount can be used
constexpr int NODE_NUM_PIXELS = 8;
constexpr double NODE_PIXEL_ANGLE = 360.0 / NODE_NUM_PIXELS;
constexpr double NODE_PIXEL_HALF_ANGLE_OFFSET = NODE_PIXEL_ANGLE/2;

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

double node_geometry::calculate_angle_between_coordinates(const Cartesian2dCoordinate &coordinateA, const Cartesian2dCoordinate &coordinateB) {
    float xa = coordinateA.getX();
    float ya = coordinateA.getY();
    float xb = coordinateB.getX();
    float yb = coordinateB.getY();

    float deltaX = xb - xa;
    float deltaY = yb - ya;

    // this seems to orient it better so we go clockwise
    double angleInRadians = std::atan2(deltaX, deltaY);

    // Convert radians to degrees
    double angleInDegrees = angleInRadians * (180.0 / M_PI);

    // Normalize the angle to [0, 360) range if necessary
    double normalizedAngle = fmod(angleInDegrees + 360.0, 360.0);

    return normalizedAngle;
}

std::vector<int> node_geometry::get_node_path(int nodeIdA, int nodeIdB, const ClusterManager &clusterManager) {

    CubeCoordinate coordA = clusterManager.getCubeCoordinate(nodeIdA);
    CubeCoordinate coordB = clusterManager.getCubeCoordinate(nodeIdB);
    int distance = get_cube_distance(coordA, coordB);

    std::vector<int> result;
    result.reserve(distance);

    for (int i = 0; i <= distance; i++) {
        std::vector<float> lerp_values = cube_lerp(coordA, coordB, static_cast<float>(1.0 / distance * i));
        CubeCoordinate rounded_values = cube_round(lerp_values[0], lerp_values[1], lerp_values[2]);
        result.push_back(clusterManager.getNodeId(rounded_values));
    }
    return result;
}

std::vector<int> get_node_neighbors_at_level(int nodeId, const ClusterManager &clusterManager, int level) {
    if (level == 0) return {nodeId};

    std::vector<int> result;
    // Calculate expected size based on level and reserve space in vector
    int perimeterSize = 6 * level; // Adjust this formula based on your grid's geometry
    result.reserve(perimeterSize);

    CubeCoordinate current = clusterManager.getCubeCoordinate(nodeId);

    for(int i = 0; i < level; i++) {
        current = current + CubeCoordinate::North();
    }
    //    CubeCoordinate current = start;
    std::vector<CubeCoordinate> directions = {CubeCoordinate::SouthEast(), CubeCoordinate::South(), CubeCoordinate::SouthWest(), CubeCoordinate::NorthWest(), CubeCoordinate::North(), CubeCoordinate::NorthEast()};
    
    // Iterate over each direction
    for (const auto &direction : directions) {
        int neighborId = clusterManager.getNodeId(current);

        // Walk 'level' steps in the current direction
        for (int i = 0; i < level; ++i) {
            result.push_back(neighborId);
            current = current + direction;
            neighborId = clusterManager.getNodeId(current);
            // Add to result, using placeholder for invalid nodes
            //result.push_back(neighborId);
        }
    }

    return result;
}

std::vector<int> node_geometry::get_node_neighbors(int nodeId, const ClusterManager &clusterManager, int level) {
    return get_node_neighbors_at_level(nodeId, clusterManager, level);
}
/*
std::vector<int> node_geometry::get_node_neighbors_v1(int nodeId, const ClusterManager &clusterManager, int level) {
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
*/
std::tuple<int, int> node_geometry::get_facing_pixel_indexes(int nodeIdA, int nodeIdB, ClusterManager &clusterManager) {
    Cartesian2dCoordinate coordA = clusterManager.getCartesian2dCoordinate(nodeIdA);
    Cartesian2dCoordinate coordB = clusterManager.getCartesian2dCoordinate(nodeIdB);

    double angle = calculate_angle_between_coordinates(coordA, coordB);
    // Ensure angle is normalized to [0, 360) if calculate_angle_between_coordinates doesn't already do this

    int pixelIndexA = static_cast<int>((angle + NODE_PIXEL_HALF_ANGLE_OFFSET) / NODE_PIXEL_ANGLE) % NODE_NUM_PIXELS;
    // Dynamically determine the opposite pixel, considering even number of pixels
    int pixelIndexB = (pixelIndexA + NODE_NUM_PIXELS / 2) % NODE_NUM_PIXELS;

    return {pixelIndexA, pixelIndexB};
}

// Map angle to pixel index
