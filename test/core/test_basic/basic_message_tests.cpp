#include "core/ClusterCommands.h"
#include "core/ClusterMessage.h"
#include "core/SensorMessage.h"
#include <iostream>
#include <unity.h>
#include <variant>

// #include "../test_utils.h"

void test_basic_message_creation() {
    FillNodeCommand fillNodeCommand(10, 0x00112233);
    ClusterMessage clusterMessage(1, FillNodeCommand::getType(), fillNodeCommand.getParams());
    NodeWithColorParams paramsInput = fillNodeCommand.getParams();
    std::vector<uint8_t> serialized = serializeClusterMessage(clusterMessage);
    ClusterMessage deserialized = deserializeClusterMessage(serialized);
    CommandParamsVariant paramsVariant = deserialized.getParams();

    TEST_ASSERT_TRUE(std::holds_alternative<NodeWithColorParams>(paramsVariant));
    NodeWithColorParams params = std::get<NodeWithColorParams>(paramsVariant);
    TEST_ASSERT_EQUAL_INT(params.nodeId, paramsInput.nodeId);
    TEST_ASSERT_EQUAL_INT(1, deserialized.getClusterId());
}

void test_node_color_buffer_message() {
    WRGB PAD_COLOR = 0x87654321;
    BlitNodeCommand nodeCommand(5, {0x12345678, 0x12345678, 0x12345678, 0x12345678, 0x12345678, 0x12345678, 0x12345678, 0x12345678}, PAD_COLOR);
    ClusterMessage clusterMessage(1, BlitNodeCommand::getType(), nodeCommand.getParams());
    std::vector<uint8_t> serialized = serializeClusterMessage(clusterMessage);
    ClusterMessage deserialized = deserializeClusterMessage(serialized);
    auto params = deserialized.getParams();
    int nodeId = std::get<BlitNodeParams>(params).nodeId;
    TEST_ASSERT_TRUE(std::holds_alternative<BlitNodeParams>(params));
    WRGB padColor = std::get<BlitNodeParams>(params).padColor;
    TEST_ASSERT_EQUAL_UINT32(padColor, PAD_COLOR);
}

void test_cluster_color_buffer_message() {
    WRGB PAD_COLOR = 0x87654321;
    std::vector<WRGB> clusterFill(80, 0x12345678);
    BlitBufferCommand nodeCommand(clusterFill, 0x87654321);
    ClusterMessage clusterMessage(1, BlitBufferCommand::getType(), nodeCommand.getParams());
    std::vector<uint8_t> serialized = serializeClusterMessage(clusterMessage);
    ClusterMessage deserialized = deserializeClusterMessage(serialized);
    auto params = deserialized.getParams();

    TEST_ASSERT_TRUE(std::holds_alternative<BlitBufferParams>(params));
    WRGB padColor = std::get<BlitBufferParams>(params).padColor;
    std::vector<WRGB> colors = std::get<BlitBufferParams>(params).colors;
    // outputBufferAsHex(colors);
    // outputBufferAsHex({padColor, PAD_COLOR});

    TEST_ASSERT_EQUAL_UINT32(padColor, PAD_COLOR);
}

void test_sensor_message() {
    TouchEventMessage message;
    message.setSensorData(1, 100, true);
    std::vector<uint8_t> serialized = message.serialize();
    std::unique_ptr<SensorMessage> sensorMessage = deserializeSensorMessage(serialized);
    const SensorEventDataProto payload = sensorMessage->getPayload();
    TEST_ASSERT_EQUAL(payload.event_type, SensorEventDataProto_EventType_TOUCH_EVENT);
    TEST_ASSERT_EQUAL_UINT32(1, payload.sensor_data.node_id);
}

int run_basic_message_tests(int argc, char **argv) {
    RUN_TEST(test_basic_message_creation);
    RUN_TEST(test_node_color_buffer_message);
    RUN_TEST(test_cluster_color_buffer_message);
    RUN_TEST(test_sensor_message);
    return 0;
}