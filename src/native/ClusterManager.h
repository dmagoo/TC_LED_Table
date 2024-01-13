// ClusterManager.h
#ifndef CLUSTERMANAGER_H
#define CLUSTERMANAGER_H

#include "ClusterConfig.h"
#include "Cluster.h"
#include <vector>
#include <unordered_map>

class ClusterManager {
private:
    std::vector<Cluster> clusters; // Vector to maintain the order of clusters
    std::unordered_map<int, size_t> clusterIdToIndexMap; // Map from clusterId to index in the vector

public:
    ClusterManager(const std::vector<ClusterConfig> &clusterConfigs);
    const Cluster* getClusterById(int clusterId) const;

    // Additional methods for ClusterManager...
};

#endif // CLUSTERMANAGER_H
