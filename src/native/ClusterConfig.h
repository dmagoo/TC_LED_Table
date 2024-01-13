
// ClusterManager.h
#ifndef CLUSTERCONFIG_H
#define CLUSTERCONFIG_H

#include "NodeConfig.h"
#include <vector>

struct ClusterConfig {
    int clusterId;
    std::vector<NodeConfig> nodeConfigs; // List of NodeConfig objects for this cluster

    ClusterConfig(int id, std::vector<NodeConfig> configs)
        : clusterId(id), nodeConfigs(std::move(configs)) {}
};

#endif // CLUSTERCONFIG_H