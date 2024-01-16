#ifndef MAKE_CLUSTER_CONFIG_H
#define MAKE_CLUSTER_CONFIG_H

#include <vector>
#include "native/ClusterConfig.h"
#include "native/NodeConfig.h"
#include "native/coordinates/RingCoordinate.h"
#include "native/coordinates/CubeCoordinate.h"
#include "native/coordinates/Cartesian2dCoordinate.h"

std::vector<ClusterConfig> makeClusterConfigs();
std::vector<NodeConfig> makeNodeConfigs(int clusterId = 0);

#endif // MAKE_CLUSTER_CONFIG_H