#include "native/ClusterCommands.h"
#include "native/ClusterMessage.h"
#include <iostream>
#include <variant>
#include <unity.h>

void test_basic_message_creation() {
    FillNodeCommand fillNodeCommand(10, 0x00112233);
    ClusterMessage clusterMessage(1, FillNodeCommand::getType(), fillNodeCommand.getParams());

    FillNodeParams paramsInput = fillNodeCommand.getParams();

    // todo: assert == 10
    // std::cout << " params --- " << (int)paramsInput.nodeId << std::endl;

    std::vector<uint8_t> serialized = serializeClusterMessage(clusterMessage);

    ClusterMessage deserialized = deserializeClusterMessage(serialized);

    // std::cout << "params" << deserialized.getParams().nodeId << std::endl;

    CommandParamsVariant paramsVariant = deserialized.getParams();

    // Step 2: Check if the variant holds an instance of FillNodeParams
    if (std::holds_alternative<FillNodeParams>(paramsVariant)) {
        // Step 3: Extract FillNodeParams
        FillNodeParams params = std::get<FillNodeParams>(paramsVariant);

        // Step 4: Compare nodeId
        if (params.nodeId == paramsInput.nodeId) {
            std::cout << "Verification successful: nodeId matches." << std::endl;
        } else {
            std::cout << "Verification failed: nodeId does not match. See: " << params.nodeId << std::endl;
        }
    } else {
        std::cout << "Deserialized command is not a FillNodeCommand." << std::endl;
    }

    auto params = deserialized.getParams();
    if (std::holds_alternative<FillNodeParams>(params)) {
        int nodeId = std::get<FillNodeParams>(params).nodeId;

        TEST_ASSERT_EQUAL_INT(10, nodeId);

        // Use nodeId as needed
    } else {
        // Handle the case where the params are not of type FillNodeParams
    }

    TEST_ASSERT_EQUAL_INT(1, deserialized.getClusterId());
    // TEST_ASSERT_EQUAL_INT(10, deserialized.getParams().nodeId);
}

int run_basic_message_tests(int argc, char **argv) {
    RUN_TEST(test_basic_message_creation);
    return 0;
}