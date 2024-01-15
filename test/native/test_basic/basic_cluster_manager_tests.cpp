#include <iostream>
#include <unity.h>
#include <vector>

#include "native/Cluster.h"
#include "native/Node.h"
#include "native/NodeConfig.h"
#include "native/ClusterManager.h"
#include "windows/AsciiUtils.h"

#include "../make_cluster_config.h"

#define PIXELS_PER_NODE 8

void test_create_cluster_manager() {
    ClusterManager clusterManager(makeClusterConfigs());
    const Cluster* cluster = clusterManager.getClusterById(0);

    TEST_ASSERT_NOT_NULL(cluster);

    const Node *node = cluster->getNode(0);
    TEST_ASSERT_EQUAL_INT(0, node->getId());

    node = cluster->getNode(1);
    TEST_ASSERT_EQUAL_INT(1, node->getId());
}

void test_ring_coordinates() {
    ClusterManager clusterManager(makeClusterConfigs());
    const Cluster* cluster = clusterManager.getClusterById(0);

    int nodeId;

    RingCoordinate coordinate(0,0);
    nodeId = clusterManager.getNodeId(coordinate);
    TEST_ASSERT_EQUAL_INT(nodeId, 0);
    RingCoordinate reverseCoordinate = clusterManager.getRingCoordinate(nodeId);
    TEST_ASSERT_EQUAL_INT(reverseCoordinate.getIndex(), 0);
    TEST_ASSERT_EQUAL_INT(reverseCoordinate.getRing(), 0);

    coordinate = RingCoordinate(2,2);
    nodeId = clusterManager.getNodeId(coordinate);
    TEST_ASSERT_EQUAL_INT(nodeId, 14);
    reverseCoordinate = clusterManager.getRingCoordinate(nodeId);
    TEST_ASSERT_EQUAL_INT(reverseCoordinate.getIndex(), 2);
    TEST_ASSERT_EQUAL_INT(reverseCoordinate.getRing(), 2);
}

void test_cartesian_2d_coordinates() {
    ClusterManager clusterManager(makeClusterConfigs());
    const Cluster* cluster = clusterManager.getClusterById(0);

    int nodeId;

    Cartesian2dCoordinate coordinate(0, 0);
    nodeId = clusterManager.getNodeId(coordinate);
    TEST_ASSERT_EQUAL_INT(nodeId, 0);
    Cartesian2dCoordinate reverseCoordinate = clusterManager.getCartesian2dCoordinate(nodeId);
    TEST_ASSERT_EQUAL_INT(reverseCoordinate.getX(), 0);
    TEST_ASSERT_EQUAL_INT(reverseCoordinate.getY(), 0);

    coordinate = Cartesian2dCoordinate(200,100);
    nodeId = clusterManager.getNodeId(coordinate);
    TEST_ASSERT_EQUAL_INT(nodeId, 14);
    reverseCoordinate = clusterManager.getCartesian2dCoordinate(nodeId);
    TEST_ASSERT_EQUAL_INT(reverseCoordinate.getX(), 200);
    TEST_ASSERT_EQUAL_INT(reverseCoordinate.getY(), 100);
}

void test_cube_coordinates() {
    ClusterManager clusterManager(makeClusterConfigs());
    const Cluster* cluster = clusterManager.getClusterById(0);

    int nodeId;

    CubeCoordinate coordinate(0, 0, 0);
    nodeId = clusterManager.getNodeId(coordinate);
    TEST_ASSERT_EQUAL_INT(nodeId, 0);
    CubeCoordinate reverseCoordinate = clusterManager.getCubeCoordinate(nodeId);
    TEST_ASSERT_EQUAL_INT(reverseCoordinate.getQ(), 0);
    TEST_ASSERT_EQUAL_INT(reverseCoordinate.getR(), 0);
    TEST_ASSERT_EQUAL_INT(reverseCoordinate.getS(), 0);

    coordinate = CubeCoordinate(2,-2, 0);
    nodeId = clusterManager.getNodeId(coordinate);
    TEST_ASSERT_EQUAL_INT(nodeId, 14);
    reverseCoordinate = clusterManager.getCubeCoordinate(nodeId);
    TEST_ASSERT_EQUAL_INT(reverseCoordinate.getQ(), 2);
    TEST_ASSERT_EQUAL_INT(reverseCoordinate.getR(), -2);
    TEST_ASSERT_EQUAL_INT(reverseCoordinate.getS(), 0);
}

int run_basic_cluster_manager_tests(int argc, char **argv) {
    RUN_TEST(test_create_cluster_manager);
    RUN_TEST(test_ring_coordinates);
    RUN_TEST(test_cartesian_2d_coordinates);
    return 0;
}