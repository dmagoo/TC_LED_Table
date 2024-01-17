#include <cstdint>
#include <iostream>
#include <unity.h>
#include <vector>

#include "../test_utils.h"
#include "api/LedTableApi.h"
#include "config/make_cluster_config.h"
#include "native/Cluster.h"
#include "native/ClusterManager.h"
#include "native/LedTableTypes.h"
#include "windows/AsciiUtils.h"

void test_basic_api_polymorphism() {
    // some tests to make sure the functions work with different
    // coordinate systems
    ClusterManager clusterManager(makeClusterConfigs());
    LedTableApi api(clusterManager);

    RingCoordinate coordinate(0, 0);
    int nodeId = clusterManager.getNodeId(coordinate);
    const Cluster *cluster = clusterManager.getClusterById(0);

    api.fillNode(coordinate, 0xFF0000FF);
    std::vector<RGBW> buffer = cluster->getNodePixelBuffer(nodeId);
    verifyBufferIsFilledWithColor(buffer, 0xFF0000FF);

    api.fillNode(coordinate, 0xFFFF0000);
    buffer = cluster->getNodePixelBuffer(nodeId);
    verifyBufferIsFilledWithColor(buffer, 0xFFFF0000);

    Cartesian2dCoordinate cartCoordinate(0, 0);
    api.fillNode(cartCoordinate, 0xFF0000FF);
    // coordinate should translate to same nodeId
    buffer = cluster->getNodePixelBuffer(nodeId);
    verifyBufferIsFilledWithColor(buffer, 0xFF0000FF);

    cartCoordinate = Cartesian2dCoordinate(100, 150);
    api.fillNode(cartCoordinate, 0xFFFFFFFF);
    buffer = cluster->getNodePixelBuffer(nodeId);
    verifyBufferIsFilledWithColor(buffer, 0xFF0000FF);

    CubeCoordinate cubeCoordinate(0, 0, 0);
    api.fillNode(cubeCoordinate, 0xFFFFFFFF);
    buffer = cluster->getNodePixelBuffer(nodeId);
    verifyBufferIsFilledWithColor(buffer, 0xFFFFFFFF);
}

void test_set_pixel() {
    ClusterManager clusterManager(makeClusterConfigs());
    LedTableApi api(clusterManager);

    RingCoordinate coordinate(3, 6);
    int nodeId = clusterManager.getNodeId(coordinate);

    std::cout << "found node id: " << nodeId << std::endl;

    int clusterId = clusterManager.getClusterIdFromNodeId(19);

    std::cout << "found cluster id: " << clusterId << std::endl;

    const Cluster *cluster = clusterManager.getClusterById(clusterId);

    if (!cluster) {
        UNITY_TEST_FAIL(__LINE__, ("invalid cluster"));
        return;
    }

    api.fillNode(nodeId, 0x00000000);
    api.setNodePixel(nodeId, 6, 0xFFFFFFFF);
    // std::cout << nodeBufferToAscii(*cluster, nodeId);

    std::vector<RGBW> buffer = cluster->getNodePixelBuffer(nodeId);
    for (int i = 0; i < buffer.size(); i++) {
        RGBW color = i == 6 ? 0xFFFFFFFF : 0x00000000;
        verifyBufferHasColorAt(buffer, i, color);
    }

    // api.setNodePixel(nodeId, 1, 0xFFFFFFFF);
    // api.queueNodePixel(nodeId, 0xFFFFFFFF);
    // api.queueNodePixel(nodeId, 0xFFFFFFFF);
    // api.queueNodePixel(nodeId, 0xFFFFFFFF);
    // api.queueNodePixel(nodeId, 0x00000000);
    // api.queueNodePixel(nodeId, 0x00000000);
    // api.dequeueNodePixel(nodeId, 0x0000FFFF);
    // std::cout << nodeBufferToAscii(*cluster, nodeId);
}

void test_fill_node() {
    // some tests to make sure the functions work with different
    // coordinate systems
    ClusterManager clusterManager(makeClusterConfigs());
    LedTableApi api(clusterManager);

    RingCoordinate coordinate(3, 5);
    int nodeId = clusterManager.getNodeId(coordinate);
    int clusterId = clusterManager.getClusterIdFromNodeId(nodeId);
    const Cluster *cluster = clusterManager.getClusterById(clusterId);

    // a simple, single-color fill
    api.fillNode(coordinate, 0xABCDEF00);
    std::vector<RGBW> buffer = cluster->getNodePixelBuffer(nodeId);
    verifyBufferIsFilledWithColor(buffer, 0xABCDEF00);

    // verify that padding works as expected
    api.fillNode(coordinate, {0xFFFFFFFF}, 0x00000000);
    buffer = cluster->getNodePixelBuffer(nodeId);

    outputBufferAsHex(buffer);

    for (int i = 0; i < buffer.size(); i++) {
        RGBW color = i == 0 ? 0xFFFFFFFF : 0x00000000;
        verifyBufferHasColorAt(buffer, i, color);
    }

    // verify that too many values will gracefully work
    api.fillNode(coordinate, {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, 0x00000000);
    buffer = cluster->getNodePixelBuffer(nodeId);

    verifyBufferIsFilledWithColor(buffer, 0xFFFFFFFF);
}

int run_basic_api_tests(int argc, char **argv) {
    RUN_TEST(test_basic_api_polymorphism);
    RUN_TEST(test_set_pixel);
    RUN_TEST(test_fill_node);
    return 0;
}