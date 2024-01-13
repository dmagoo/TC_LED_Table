#include "test_utils.h"

std::vector<NodeConfig> makeNodeConfigs(int clusterId) {
    switch(clusterId) {
    case 0:
        return {
            NodeConfig(0, 8, {0, 0}, {0, 0, 0}, {0, 0}),
            NodeConfig(1, 8, {1, 0}, {-1, 1, 0}, {0, 1}),
            NodeConfig(2, 8, {1, 1}, {0, 1, -1}, {1, 1}),
            NodeConfig(3, 8, {1, 2}, {1, 0, -1}, {1, 5}),
            NodeConfig(4, 8, {1, 3}, {1, 0, -1}, {1, 5}),
            NodeConfig(5, 8, {1, 4}, {1, 0, -1}, {1, 5}),
            NodeConfig(6, 8, {1, 5}, {1, 0, -1}, {1, 5})
        };
    case 1:
        return {
            NodeConfig(7, 8, {2, 11}, {0, 0, 0}, {0, 0}),
            NodeConfig(8, 8, {3, 17}, {-1, 1, 0}, {0, 1}),
            NodeConfig(9, 8, {3, 0}, {0, 1, -1}, {1, 1}),
            NodeConfig(10, 8, {2, 0}, {1, 0, -1}, {1, 5}),
            NodeConfig(11, 8, {3, 1}, {1, 0, -1}, {1, 5}),
            NodeConfig(12, 8, {2, 1}, {1, 0, -1}, {1, 5}),
            NodeConfig(13, 8, {3, 2}, {1, 0, -1}, {1, 5}),
            NodeConfig(14, 8, {3, 2}, {1, 0, -1}, {1, 5}),
            NodeConfig(15, 8, {3, 3}, {1, 0, -1}, {1, 5}),
            NodeConfig(16, 8, {3, 4}, {1, 0, -1}, {1, 5})
        };
    }

    return {};
}

std::vector<ClusterConfig> makeClusterConfigs() {
    return {
        ClusterConfig(0, makeNodeConfigs(0)),
        ClusterConfig(1, makeNodeConfigs(1))
    };
}