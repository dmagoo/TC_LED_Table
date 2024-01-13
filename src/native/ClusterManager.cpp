#include "ClusterManager.h"

ClusterManager::ClusterManager(const std::vector<ClusterConfig> &clusterConfigs) {
    for (const auto &clusterConfig : clusterConfigs) {
        clusters.emplace_back(clusterConfig.clusterId, clusterConfig.nodeConfigs);
        clusterIdToIndexMap[clusterConfig.clusterId] = clusters.size() - 1;
    }
}

const Cluster *ClusterManager::getClusterById(int clusterId) const {
    auto it = clusterIdToIndexMap.find(clusterId);
    if (it != clusterIdToIndexMap.end()) {
        return &clusters[it->second];
    }
    return nullptr; // Or handle the error as appropriate
}
