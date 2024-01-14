#ifndef MAKE_CLUSTER_CONFIG_H
#define MAKE_CLUSTER_CONFIG_H

#include <vector>
#include "native/ClusterConfig.h"
#include "native/NodeConfig.h"

std::vector<ClusterConfig> makeClusterConfigs();
std::vector<NodeConfig> makeNodeConfigs(int clusterId = 0);

#endif // MAKE_CLUSTER_CONFIG_H