#ifndef MAKE_CLUSTER_CONFIG_H
#define MAKE_CLUSTER_CONFIG_H

#include <vector>
#include "core/ClusterConfig.h"
#include "core/NodeConfig.h"
#include "core/coordinates/RingCoordinate.h"
#include "core/coordinates/CubeCoordinate.h"
#include "core/coordinates/Cartesian2dCoordinate.h"

std::vector<ClusterConfig> makeClusterConfigs();
std::vector<ClusterConfig> makeClusterConfig(int clusterId);
std::vector<NodeConfig> makeNodeConfigs(int clusterId = 0);

#endif // MAKE_CLUSTER_CONFIG_H