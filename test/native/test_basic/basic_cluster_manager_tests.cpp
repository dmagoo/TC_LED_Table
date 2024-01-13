#include <iostream>
#include <unity.h>
#include <vector>

#include "native/Cluster.h"
#include "native/Node.h"
#include "native/NodeConfig.h"
#include "native/ClusterManager.h"
#include "windows/AsciiUtils.h"

#include "../test_utils.h"


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

int run_basic_cluster_manager_tests(int argc, char **argv) {
    RUN_TEST(test_create_cluster_manager);
    return 0;
}