#ifndef NODE_GEOMETRY_H
#define NODE_GEOMETRY_H

#include <cmath>
#include <vector>

#include "ClusterManager.h"
#include "coordinates/CubeCoordinate.h"

namespace node_geometry {
/* Generates a path between two nodes by interpolating between their cube coordinates and rounding to the nearest valid cube coordinates at each step. */
std::vector<int> get_node_path(int nodeIdA, int nodeIdB, const ClusterManager &clusterManager);

/* Retrieves the IDs of neighboring nodes for a given node, including both valid and invalid neighbors to maintain their positions starting from the North direction. */
std::vector<int> get_node_neighbors(int nodeId, const ClusterManager &clusterManager);

// returns a tuple of two pixel indexes, indicating which pixel faces the other node
std::tuple<int, int> get_facing_pixel_indexes(int nodeIdA, int nodeIdB, ClusterManager &ClusterManager);

/* Linearly interpolate between two integer values based on a float parameter t, useful for finding intermediate points. */
float lerp(int a, int b, float t);

/* Perform linear interpolation between two CubeCoordinate points, returning a vector of interpolated float values for q, r, and s coordinates. */
std::vector<float> cube_lerp(const CubeCoordinate &coordA, const CubeCoordinate &coordB, float t);

/* Rounds floating-point cube coordinates to the nearest integer coordinates, with logic to maintain the cube coordinate constraint */
CubeCoordinate cube_round(float q_float, float r_float, float s_float);

/* Calculates the distance between two cube coordinates*/
int get_cube_distance(CubeCoordinate coordA, CubeCoordinate coordB);

// returns the angle in degrees
double calculate_angle_between_coordinates(const Cartesian2dCoordinate &coordinateA, const Cartesian2dCoordinate &coordinateB);

} // namespace node_geometry

#endif // NODE_GEOMETRY_H