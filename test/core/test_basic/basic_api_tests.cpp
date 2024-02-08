#include <cstdint>
#include <iostream>
#include <unity.h>
#include <vector>

#include "../test_utils.h"
#include "api/LedTableApi.h"
#include "config/make_cluster_config.h"
// #include "config/make_mqtt_config.h"
#include "config/led_table_config.h"
#include "core/Cluster.h"
#include "core/ClusterCommands.h"
#include "core/ClusterManager.h"
#include "core/ClusterMessageManager.h"
#include "core/LedTableTypes.h"
#include "windows/AsciiUtils.h"

void test_basic_api_overloading() {
    // some tests to make sure the functions work with different
    // coordinate systems
    ClusterManager clusterManager(makeClusterConfigs());

//    auto mqttClient = makeMQTTClientConfig();
//    ClusterMessageManager clusterMessageManager(mqttClient.get());

//    LedTableApi api(clusterManager, &clusterMessageManager);


    // In the context of using this configuration
    LedTableConfig config;
    config.mqttConfig.brokerAddress = "tcp://192.168.1.49";
    config.enableMessaging = true;

    LedTableApi api(clusterManager, config);

    // we can circumvent messaging all together by omitting the message manager: LedTableApi api(clusterManager);

    // we can temporarily disable messages, so we can push out led state in a more organized manner
    api.setSuppressMessages(true);
    RingCoordinate coordinate(0, 0);
    int nodeId = clusterManager.getNodeId(coordinate);
    const Cluster *cluster = clusterManager.getClusterById(0);

    api.fillNode(coordinate, 0xFF0000FF);
    std::vector<WRGB> buffer = cluster->getNodePixelBuffer(nodeId);
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
    api.setSuppressMessages(true);
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

    std::vector<WRGB> buffer = cluster->getNodePixelBuffer(nodeId);
    for (int i = 0; i < buffer.size(); i++) {
        WRGB color = i == 6 ? 0xFFFFFFFF : 0x00000000;
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
    api.setSuppressMessages(true);
    RingCoordinate coordinate(3, 5);
    int nodeId = clusterManager.getNodeId(coordinate);
    int clusterId = clusterManager.getClusterIdFromNodeId(nodeId);
    const Cluster *cluster = clusterManager.getClusterById(clusterId);

    // a simple, single-color fill
    api.fillNode(coordinate, 0xABCDEF00);
    std::vector<WRGB> buffer = cluster->getNodePixelBuffer(nodeId);
    verifyBufferIsFilledWithColor(buffer, 0xABCDEF00);

    // verify that padding works as expected
    api.fillNode(coordinate, {0xFFFFFFFF}, 0x00000000);
    buffer = cluster->getNodePixelBuffer(nodeId);

    outputBufferAsHex(buffer);

    for (int i = 0; i < buffer.size(); i++) {
        WRGB color = i == 0 ? 0xFFFFFFFF : 0x00000000;
        verifyBufferHasColorAt(buffer, i, color);
    }

    // verify that too many values will gracefully work
    api.fillNode(coordinate, {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, 0x00000000);
    buffer = cluster->getNodePixelBuffer(nodeId);

    verifyBufferIsFilledWithColor(buffer, 0xFFFFFFFF);
}

void test_reset_cluster() {
    ClusterManager clusterManager(makeClusterConfigs());
    LedTableApi api(clusterManager);
    api.setSuppressMessages(true);

    for (int i = 0; i < 7; i++) {
        api.fillNode(i, 0xFFFFFFFF);
    }

    const Cluster *cluster = clusterManager.getClusterById(0);
    std::vector<WRGB> buffer = cluster->getPixelBuffer();
    verifyBufferIsFilledWithColor(buffer, 0xFFFFFFFF);

    api.reset();

    buffer = cluster->getPixelBuffer();
    verifyBufferIsFilledWithColor(buffer, 0x00000000);
}

void test_node_geometry() {
    ClusterManager clusterManager(makeClusterConfigs());
    LedTableApi api(clusterManager);
    std::vector<int> neighbors = api.getNodeNeighbors(0);
    std::vector<int> expected = {1, 2, 3, 4, 5, 6};
    TEST_ASSERT_EQUAL_INT_ARRAY(expected.data(), neighbors.data(), expected.size());

    neighbors = api.getNodeNeighbors(7);
    expected = {8, 10, 1, 6, 34, 36};
    TEST_ASSERT_EQUAL_INT_ARRAY(expected.data(), neighbors.data(), expected.size());

    neighbors = api.getNodeNeighbors(23);
    expected = {22, 21, -1, -1, 25, 24};
    TEST_ASSERT_EQUAL_INT_ARRAY(expected.data(), neighbors.data(), expected.size());
    std::cout << std::dec;
    neighbors = api.getNodeNeighbors(9);
    expected = {-1, -1, 11, 10, 8, -1};
    TEST_ASSERT_EQUAL_INT_ARRAY(expected.data(), neighbors.data(), expected.size());

    std::cout << "9 to 25" << std::endl;
    std::vector<int> path = api.getNodePath(9, 25);

    std::cout << "PATH: " << std::dec;
    std::copy(path.begin(), path.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    expected = {9, 10, 1, 0, 4, 24, 25};
    TEST_ASSERT_EQUAL_INT_ARRAY(expected.data(), path.data(), expected.size());

    std::cout << "33 to 2" << std::endl;
    path = api.getNodePath(33, 2);
    expected = {33, 34, 6, 1, 2};
    std::cout << "PATH: " << std::dec;
    std::copy(path.begin(), path.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    TEST_ASSERT_EQUAL_INT_ARRAY(expected.data(), path.data(), expected.size());

    std::cout << "35 to 9" << std::endl;
    path = api.getNodePath(35, 19);
    expected = {35, 34, 6, 0, 3, 20, 19};

    std::cout << "PATH: " << std::dec;
    std::copy(path.begin(), path.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    TEST_ASSERT_EQUAL_INT_ARRAY(expected.data(), path.data(), expected.size());


    std::tuple<int, int> result = api.getFacingPixelIndexes(RingCoordinate(0, 0), RingCoordinate(3, 0));
    int pixelIndexA = std::get<0>(result);  // Access first element
    int pixelIndexB = std::get<1>(result);  // Access second element
    std::cout << "pixel: " << pixelIndexA << " faces " << pixelIndexB << std::endl;

    result = api.getFacingPixelIndexes(RingCoordinate(0, 0), RingCoordinate(1, 0));
    pixelIndexA = std::get<0>(result);  // Access first element
    pixelIndexB = std::get<1>(result);  // Access second element
    TEST_ASSERT_EQUAL_INT(0, pixelIndexA);
    TEST_ASSERT_EQUAL_INT(4, pixelIndexB);


    result = api.getFacingPixelIndexes(RingCoordinate(0, 0), RingCoordinate(1, 1));
    pixelIndexA = std::get<0>(result);  // Access first element
    pixelIndexB = std::get<1>(result);  // Access second element
    TEST_ASSERT_EQUAL_INT(1, pixelIndexA);
    TEST_ASSERT_EQUAL_INT(5, pixelIndexB);

    result = api.getFacingPixelIndexes(RingCoordinate(0, 0), RingCoordinate(1, 2));
    pixelIndexA = std::get<0>(result);  // Access first element
    pixelIndexB = std::get<1>(result);  // Access second element
    TEST_ASSERT_EQUAL_INT(3, pixelIndexA);
    TEST_ASSERT_EQUAL_INT(7, pixelIndexB);

    result = api.getFacingPixelIndexes(RingCoordinate(0, 0), RingCoordinate(1, 3));
    pixelIndexA = std::get<0>(result);  // Access first element
    pixelIndexB = std::get<1>(result);  // Access second element
    TEST_ASSERT_EQUAL_INT(4, pixelIndexA);
    TEST_ASSERT_EQUAL_INT(0, pixelIndexB);

    result = api.getFacingPixelIndexes(RingCoordinate(0, 0), RingCoordinate(1, 4));
    pixelIndexA = std::get<0>(result);  // Access first element
    pixelIndexB = std::get<1>(result);  // Access second element
    TEST_ASSERT_EQUAL_INT(5, pixelIndexA);
    TEST_ASSERT_EQUAL_INT(1, pixelIndexB);

    result = api.getFacingPixelIndexes(RingCoordinate(0, 0), RingCoordinate(1, 5));
    pixelIndexA = std::get<0>(result);  // Access first element
    pixelIndexB = std::get<1>(result);  // Access second element
    TEST_ASSERT_EQUAL_INT(7, pixelIndexA);
    TEST_ASSERT_EQUAL_INT(3, pixelIndexB);
    
    result = api.getFacingPixelIndexes(RingCoordinate(0, 0), RingCoordinate(2, 3));
    pixelIndexA = std::get<0>(result);  // Access first element
    pixelIndexB = std::get<1>(result);  // Access second element
    TEST_ASSERT_EQUAL_INT(2, pixelIndexA);
    TEST_ASSERT_EQUAL_INT(6, pixelIndexB);


    result = api.getFacingPixelIndexes(RingCoordinate(3, 4), RingCoordinate(2, 7));
    pixelIndexA = std::get<0>(result);  // Access first element
    pixelIndexB = std::get<1>(result);  // Access second element
    TEST_ASSERT_EQUAL_INT(5, pixelIndexA);
    TEST_ASSERT_EQUAL_INT(1, pixelIndexB);

    result = api.getFacingPixelIndexes(RingCoordinate(2, 7), RingCoordinate(3, 4));
    pixelIndexA = std::get<0>(result);  // Access first element
    pixelIndexB = std::get<1>(result);  // Access second element
    TEST_ASSERT_EQUAL_INT(1, pixelIndexA);
    TEST_ASSERT_EQUAL_INT(5, pixelIndexB);

    // longer distances
    result = api.getFacingPixelIndexes(RingCoordinate(3, 0), RingCoordinate(3, 8));
    pixelIndexA = std::get<0>(result);  // Access first element
    pixelIndexB = std::get<1>(result);  // Access second element
    TEST_ASSERT_EQUAL_INT(4, pixelIndexA);
    TEST_ASSERT_EQUAL_INT(0, pixelIndexB);

    result = api.getFacingPixelIndexes(RingCoordinate(3, 2), RingCoordinate(3, 10));
    pixelIndexA = std::get<0>(result);  // Access first element
    pixelIndexB = std::get<1>(result);  // Access second element
    TEST_ASSERT_EQUAL_INT(5, pixelIndexA);
    TEST_ASSERT_EQUAL_INT(1, pixelIndexB);



    /*
    result = api.getFacingPixelIndexes(RingCoordinate(0, 0), RingCoordinate(1, 0));
    pixelIndexA = std::get<0>(result);  // Access first element
    pixelIndexB = std::get<1>(result);  // Access second element
    std::cout << "pixel: " << pixelIndexA << " faces " << pixelIndexB << std::endl;

    result = api.getFacingPixelIndexes(0, 9);
    pixelIndexA = std::get<0>(result);  // Access first element
    pixelIndexB = std::get<1>(result);  // Access second element
    std::cout << "0,9 pixel: " << pixelIndexA << " faces " << pixelIndexB << std::endl;

    result = api.getFacingPixelIndexes(0, 2);
    pixelIndexA = std::get<0>(result);  // Access first element
    pixelIndexB = std::get<1>(result);  // Access second element
    std::cout << "0,2 pixel: " << pixelIndexA << " faces " << pixelIndexB << std::endl;

    result = api.getFacingPixelIndexes(0, 3);
    pixelIndexA = std::get<0>(result);  // Access first element
    pixelIndexB = std::get<1>(result);  // Access second element
    std::cout << "0,3 pixel: " << pixelIndexA << " faces " << pixelIndexB << std::endl;
*/

}

int run_basic_api_tests(int argc, char **argv) {
    RUN_TEST(test_basic_api_overloading);
    RUN_TEST(test_set_pixel);
    RUN_TEST(test_fill_node);
    RUN_TEST(test_reset_cluster);
    RUN_TEST(test_node_geometry);
    return 0;
}