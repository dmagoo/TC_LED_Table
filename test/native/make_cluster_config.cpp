#include "make_cluster_config.h"

std::vector<ClusterConfig> makeClusterConfigs() {
    return {
        ClusterConfig(0, makeNodeConfigs(0)),
        ClusterConfig(1, makeNodeConfigs(1)),
        ClusterConfig(2, makeNodeConfigs(2)),
        ClusterConfig(3, makeNodeConfigs(3)),
    };
}

std::vector<NodeConfig> makeNodeConfigs(int clusterId) {
    switch(clusterId) {
    case 0:
        return {
            NodeConfig(0, 8, {0, 0}, {0, 0, 0}, {0.0, 0.0}),
            NodeConfig(1, 8, {1, 0}, {-1, 1, 0}, {100.0}),
            NodeConfig(2, 8, {1, 1}, {0, 1, -1}, {100.0, 50.0}),
            NodeConfig(3, 8, {1, 2}, {1, 0, -1}, {100.0, -50.0}),
            NodeConfig(4, 8, {1, 3}, {1, -1, 0}, {0.0, -100.0}),
            NodeConfig(5, 8, {1, 4}, {0, -1, 1}, {-100.0, -50.0}),
            NodeConfig(6, 8, {1, 5}, {-1, 0, 1}, {-100.0, 50.0}),
        };
    case 1:
        return {
            NodeConfig(7, 8, {2, 11}, {1, 1, -1}, {-100.0, 150.0}),
            NodeConfig(8, 8, {3, 17}, {2, 1, -3}, {-100.0, 250.0}),
            NodeConfig(9, 8, {3, 0}, {3, 0, -3}, {300.0}),
            NodeConfig(10, 8, {2, 0}, {2, 0, 0}, {200.0}),
            NodeConfig(11, 8, {3, 1}, {3, 1, -2}, {100.0, 250.0}),
            NodeConfig(12, 8, {2, 1}, {2, 1, -1}, {100.0, 150.0}),
            NodeConfig(13, 8, {3, 2}, {3, 2, -1}, {200.0, 200.0}),
            NodeConfig(14, 8, {2, 2}, {2, -2, 0}, {200.0, 100.0}),
            NodeConfig(15, 8, {3, 3}, {3, -3, 1}, {300.0, 150.0}),
            NodeConfig(16, 8, {3, 4}, {2, 3, 1}, {300.0, 50.0}),
        };
    case 2:
        return {
            NodeConfig(17, 8, {2, 3}, {-2, 1, 1}, {200.0, 0.0}),
            NodeConfig(18, 8, {3, 5}, {-3, 2, 1}, {300.0, -50.0}),
            NodeConfig(19, 8, {3, 6}, {-3, 3, 0}, {300.0, -150.0}),
            NodeConfig(20, 8, {2, 4}, {-2, 2, 0}, {200.0, -100.0}),
            NodeConfig(21, 8, {3, 7}, {-2, 3, -1}, {200.0, -200.0}),
            NodeConfig(22, 8, {2, 5}, {-1, 2, -1}, {100.0, -150.0}),
            NodeConfig(23, 8, {3, 8}, {-1, 3, -2}, {100.0, -250.0}),
            NodeConfig(24, 8, {2, 6}, {0, 2, -2}, {0.0, -200.0}),
            NodeConfig(25, 8, {3, 9}, {0, 3, -3}, {0.0, -300.0}),
            NodeConfig(26, 8, {3, 10}, {1, 2, 3}, {-100.0, -250.0}),
        };
    case 3:
        return {
            NodeConfig(27, 8, {2, 7}, {1, 2, 1}, {-100.0, -150.0}),
            NodeConfig(28, 8, {3, 11}, {1, 3, 2}, {-200.0, -200.0}),
            NodeConfig(29, 8, {3, 12}, {0, -3, 3}, {-300.0, -150.0}),
            NodeConfig(30, 8, {2, 8}, {0, -2, 2}, {-200.0, -100.0}),
            NodeConfig(31, 8, {3, 13}, {-1, 2, 3}, {-300.0, -50.0}),
            NodeConfig(32, 8, {2, 9}, {-1, 1, 2}, {-200.0, 0.0}),
            NodeConfig(33, 8, {3, 14}, {-2, 1, 3}, {-300.0, 50.0}),
            NodeConfig(34, 8, {2, 10}, {-2, 0, 2}, {-200.0, 100.0}),
            NodeConfig(35, 8, {3, 15}, {-3, 0, 3}, {-300.0, 150.0}),
            NodeConfig(36, 8, {3, 16}, {-3, 1, 2}, {-200.0, 200.0}),
        };
    }
    return {};
}
