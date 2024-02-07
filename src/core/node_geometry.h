#ifndef NODE_GEOMETRY_H
#define NODE_GEOMETRY_H

#include <cmath>
#include <vector>

#include "ClusterManager.h"
#include "coordinates/CubeCoordinate.h"

namespace node_geometry {
std::vector<int> get_node_path(int nodeIdA, int nodeIdB, const ClusterManager &clusterManager);
std::vector<int> get_node_neighbors(int nodeId, const ClusterManager &clusterManager);

float lerp(int a, int b, float t);
std::vector<float> cube_lerp(const CubeCoordinate &coordA, const CubeCoordinate &coordB, float t);
CubeCoordinate cube_round(float q_float, float r_float, float s_float);
int get_cube_distance(CubeCoordinate coordA, CubeCoordinate coordB);
} // namespace node_geometry

#endif // NODE_GEOMETRY_H