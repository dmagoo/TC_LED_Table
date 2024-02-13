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
    //std::cout << "====================" << std::endl;

    //std::cout << " xa: " << xa << " ya: " << ya <<" xb: " <<  xb <<" yb: " <<  yb << std::endl;
    float deltaX = xb - xa;
    float deltaY = yb - ya;
    //double angleInRadians = std::atan2(deltaY, deltaX);
    // this seems to orient it beter so we go clockwise
    double angleInRadians = std::atan2(deltaX, deltaY);
    //std::cout << "radians: " << angleInRadians << std::endl;
    // Convert radians to degrees
    double angleInDegrees = angleInRadians * (180.0 / M_PI);
//std::cout << " degrees " << angleInDegrees << std::endl;
    // Normalize the angle to [0, 360) range if necessary
    double normalizedAngle = fmod(angleInDegrees + 360.0, 360.0);
//std::cout << " normalized " << normalizedAngle << std::endl;
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
