#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <vector>
#include "native/NodeConfig.h"
#include "native/ClusterConfig.h"

std::vector<NodeConfig> makeNodeConfigs(int clusterId = 0);
std::vector<ClusterConfig> makeClusterConfigs();
#endif // TEST_UTILS_H
