#include "make_cluster_config.h"

std::vector<ClusterConfig> makeClusterConfigs() {
    return {
        ClusterConfig(0, makeNodeConfigs(0)),
        ClusterConfig(1, makeNodeConfigs(1)),
        ClusterConfig(2, makeNodeConfigs(2)),
        ClusterConfig(3, makeNodeConfigs(3)),
    };
}

// allow a universe of a single cluster for unit testing
std::vector<ClusterConfig> makeClusterConfig(int clusterId) {
    return {
        ClusterConfig(clusterId, makeNodeConfigs(clusterId)),
    };
}

std::vector<NodeConfig> makeNodeConfigs(int clusterId) {
    switch(clusterId) {
    case 0:
        return {
            NodeConfig(0, 8,
                RingCoordinate(0, 0),
                CubeCoordinate(0, 0, 0),
                Cartesian2dCoordinate(0.0, 0.0)),
            NodeConfig(1, 8,
                RingCoordinate(1, 0),
                CubeCoordinate(-1, 1, 0),
                Cartesian2dCoordinate(0.0, 100.0)),
            NodeConfig(2, 8,
                RingCoordinate(1, 1),
                CubeCoordinate(0, 1, -1),
                Cartesian2dCoordinate(100.0, 50.0)),
            NodeConfig(3, 8,
                RingCoordinate(1, 2),
                CubeCoordinate(1, 0, -1),
                Cartesian2dCoordinate(100.0, -50.0)),
            NodeConfig(4, 8,
                RingCoordinate(1, 3),
                CubeCoordinate(1, -1, 0),
                Cartesian2dCoordinate(0.0, -100.0)),
            NodeConfig(5, 8,
                RingCoordinate(1, 4),
                CubeCoordinate(0, -1, 1),
                Cartesian2dCoordinate(-100.0, -50.0)),
            NodeConfig(6, 8,
                RingCoordinate(1, 5),
                CubeCoordinate(-1, 0, 1),
                Cartesian2dCoordinate(-100.0, 50.0))
        };
    case 1:
        return {
            NodeConfig(7, 8,
                RingCoordinate(2, 11),
                CubeCoordinate(-2, 1, 1),
                Cartesian2dCoordinate(-100.0, 150.0)),
            NodeConfig(8, 8,
                RingCoordinate(3, 17),
                CubeCoordinate(-3, 2, 1),
                Cartesian2dCoordinate(-100.0, 250.0)),
            NodeConfig(9, 8,
                RingCoordinate(3, 0),
                CubeCoordinate(-3, 3, 0),
                Cartesian2dCoordinate(0.0, 300.0)),
            NodeConfig(10, 8,
                RingCoordinate(2, 0),
                CubeCoordinate(-2, 2, 0),
                Cartesian2dCoordinate(0.0, 200.0)),
            NodeConfig(11, 8,
                RingCoordinate(3, 1),
                CubeCoordinate(-2, 3, -1),
                Cartesian2dCoordinate(100.0, 250.0)),
            NodeConfig(12, 8,
                RingCoordinate(2, 1),
                CubeCoordinate(-1, 2, -1),
                Cartesian2dCoordinate(100.0, 150.0)),
            NodeConfig(13, 8,
                RingCoordinate(3, 2),
                CubeCoordinate(-1, 3, -2),
                Cartesian2dCoordinate(200.0, 200.0)),
            NodeConfig(14, 8,
                RingCoordinate(2, 2),
                CubeCoordinate(0, 2, -2),
                Cartesian2dCoordinate(200.0, 100.0)),
            NodeConfig(15, 8,
                RingCoordinate(3, 3),
                CubeCoordinate(0, 3, -3),
                Cartesian2dCoordinate(300.0, 150.0)),
            NodeConfig(16, 8,
                RingCoordinate(3, 4),
                CubeCoordinate(1, 2, -3),
                Cartesian2dCoordinate(300.0, 50.0))
        };
    case 2:
        return {
            NodeConfig(17, 8,
                RingCoordinate(2, 3),
                CubeCoordinate(1, 1, -2),
                Cartesian2dCoordinate(200.0, 0.0)),
            NodeConfig(18, 8,
                RingCoordinate(3, 5),
                CubeCoordinate(2, 1, -3),
                Cartesian2dCoordinate(300.0, -50.0)),
            NodeConfig(19, 8,
                RingCoordinate(3, 6),
                CubeCoordinate(3, 0, -3),
                Cartesian2dCoordinate(300.0, -150.0)),
            NodeConfig(20, 8,
                RingCoordinate(2, 4),
                CubeCoordinate(2, 0, -2),
                Cartesian2dCoordinate(200.0, -100.0)),
            NodeConfig(21, 8,
                RingCoordinate(3, 7),
                CubeCoordinate(3, -1, -2),
                Cartesian2dCoordinate(200.0, -200.0)),
            NodeConfig(22, 8,
                RingCoordinate(2, 5),
                CubeCoordinate(2, -1, -1),
                Cartesian2dCoordinate(100.0, -150.0)),
            NodeConfig(23, 8,
                RingCoordinate(3, 8),
                CubeCoordinate(3, -2, -1),
                Cartesian2dCoordinate(100.0, -250.0)),
            NodeConfig(24, 8,
                RingCoordinate(2, 6),
                CubeCoordinate(2, -2, 0),
                Cartesian2dCoordinate(0.0, -200.0)),
            NodeConfig(25, 8,
                RingCoordinate(3, 9),
                CubeCoordinate(3, -3, 0),
                Cartesian2dCoordinate(0.0, -300.0)),
            NodeConfig(26, 8,
                RingCoordinate(3, 10),
                CubeCoordinate(1, -2, 1),
                Cartesian2dCoordinate(-100.0, -250.0))
        };
    case 3:
        return {
            NodeConfig(27, 8,
                RingCoordinate(2, 7),
                CubeCoordinate(1, 2, 1),
                Cartesian2dCoordinate(-100.0, -150.0)),
            NodeConfig(28, 8,
                RingCoordinate(3, 11),
                CubeCoordinate(1, -3, 2),
                Cartesian2dCoordinate(-200.0, -200.0)),
            NodeConfig(29, 8,
                RingCoordinate(3, 12),
                CubeCoordinate(0, -3, 3),
                Cartesian2dCoordinate(-300.0, -150.0)),
            NodeConfig(30, 8,
                RingCoordinate(2, 8),
                CubeCoordinate(0, -2, 2),
                Cartesian2dCoordinate(-200.0, -100.0)),
            NodeConfig(31, 8,
                RingCoordinate(3, 13),
                CubeCoordinate(-1, -2, 3),
                Cartesian2dCoordinate(-300.0, -50.0)),
            NodeConfig(32, 8,
                RingCoordinate(2, 9),
                CubeCoordinate(-1, -1, 2),
                Cartesian2dCoordinate(-200.0, 0.0)),
            NodeConfig(33, 8,
                RingCoordinate(3, 14),
                CubeCoordinate(-2, -1, 3),
                Cartesian2dCoordinate(-300.0, 50.0)),
            NodeConfig(34, 8,
                RingCoordinate(2, 10),
                CubeCoordinate(-2, 0, 2),
                Cartesian2dCoordinate(-200.0, 100.0)),
            NodeConfig(35, 8,
                RingCoordinate(3, 15),
                CubeCoordinate(-3, 1, 2),
                Cartesian2dCoordinate(-300.0, 150.0)),
            NodeConfig(36, 8,
                RingCoordinate(3, 16),
                CubeCoordinate(-3, 1, 2),
                Cartesian2dCoordinate(-200.0, 200.0))
        };
    }
    return {};
}
