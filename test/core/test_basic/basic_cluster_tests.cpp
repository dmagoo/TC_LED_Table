#include <iostream>
#include <unity.h>
#include <vector>

#include "windows/AsciiUtils.h"
#include "core/LedTableTypes.h"
#include "core/Cluster.h"
#include "core/Node.h"
#include "core/NodeConfig.h"

#include "config/make_cluster_config.h"

#define PIXELS_PER_NODE 8

void test_create_cluster() {
    std::vector<NodeConfig> nodeConfigs = makeNodeConfigs(0);
    Cluster cluster(0, nodeConfigs);

    const Node *node1 = cluster.getNode(0);
    const Node *node2 = cluster.getNode(1);
    const Node *node7 = cluster.getNode(6);

    TEST_ASSERT_EQUAL_INT(0, node1->getId());
    TEST_ASSERT_EQUAL_INT(1, node2->getId());
    TEST_ASSERT_EQUAL_INT(6, node7->getId());
    TEST_ASSERT_EQUAL_INT(48, node7->getStartIndex());
    TEST_ASSERT_EQUAL_INT(56, cluster.getPixelCount());
}

void test_pixel_buffer_fill_node() {
    std::vector<NodeConfig> nodeConfigs = makeNodeConfigs(0);
    Cluster cluster(0, nodeConfigs);

    RGBW color1 = 0xFF0000FF; // Example color 1
    RGBW color2 = 0x00FF00FF; // Example color 2
    for (int i = 0; i < nodeConfigs.size(); ++i) {
        cluster.fillNode(i, (i % 2 == 0) ? color1 : color2);
    }

    // Test: Check buffer size and contents
    std::vector<RGBW> buffer = cluster.getPixelBuffer();
    int totalPixelCount = PIXELS_PER_NODE * nodeConfigs.size();
    TEST_ASSERT_EQUAL_INT(totalPixelCount, buffer.size());

    for (int i = 0; i < buffer.size(); ++i) {
        int nodeIndex = (int)i / PIXELS_PER_NODE;
        RGBW expectedColor = (nodeIndex % 2 == 0) ? color1 : color2;
        TEST_ASSERT_EQUAL_HEX32(expectedColor, buffer[i]);
    }

    std::string asciiArt = nodeBufferToAscii(cluster, 0);
    std::cout << asciiArt;

    asciiArt = nodeBufferToAscii(cluster, 1);
    std::cout << asciiArt;
}

void run_basic_cluster_tests(int argc, char **argv) {
    RUN_TEST(test_create_cluster);
    RUN_TEST(test_pixel_buffer_fill_node);
}