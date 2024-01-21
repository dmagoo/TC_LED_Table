#include "ClusterMessage.h"

// # include <google/protobuf/message.h>
#include "native/proto/ClusterMessagesProto.pb.h"
// #include "ClusterMessagesProto.pb.h"
#include <iostream>
#include <vector>
#include <variant>

int ClusterMessage::getClusterId() const {
    return clusterId;
}

ClusterCommandType ClusterMessage::getCommandType() const {
    return commandType;
}

CommandParamsVariant ClusterMessage::getParams() const {
    return params;
}

ClusterMessage createFillNodeMessage(int clusterId, int nodeId, int color) {
    NodeWithColorParams params;
    params.nodeId = nodeId;
    params.color = color;
    ClusterMessage clusterMessage(clusterId, ClusterCommandType::FillNode, params);
    return clusterMessage;
}

ClusterMessage createBlitNodeMessage(int clusterId, int nodeId, std::vector<RGBW> colors, RGBW padColor) {
    BlitNodeParams params;
    params.nodeId = nodeId;
    params.colors = std::move(colors);
    params.padColor = padColor;
    ClusterMessage clusterMessage(clusterId, ClusterCommandType::BlitNode, params);
    return clusterMessage;
}

ClusterMessage createSetNodePixelMessage(int clusterId, int nodeId, int color) {
    NodeWithPixelIndex params;
    params.nodeId = nodeId;
    params.color = color;
    ClusterMessage clusterMessage(clusterId, ClusterCommandType::SetNodePixel, params);
    return clusterMessage;
}

ClusterMessage createQueueNodePixelMessage(int clusterId, int nodeId, int color) {
    NodeWithColorParams params;
    params.nodeId = nodeId;
    params.color = color;
    ClusterMessage clusterMessage(clusterId, ClusterCommandType::QueueNodePixel, params);
    return clusterMessage;
}

ClusterMessage createDequeueNodePixelMessage(int clusterId, int nodeId, int color) {
    NodeWithColorParams params;
    params.nodeId = nodeId;
    params.color = color;
    ClusterMessage clusterMessage(clusterId, ClusterCommandType::DequeueNodePixel, params);
    return clusterMessage;
}

std::vector<uint8_t> serializeClusterMessage(const ClusterMessage &message) {
    ClusterMessageProto proto_msg = ClusterMessageProto_init_zero;

    // Set fields from your ClusterMessage to proto_msg
    proto_msg.cluster_id = message.getClusterId();
    proto_msg.command_type = static_cast<CommandTypeProto>(message.getCommandType());
    // Handle setting specific command params based on command_type

    // Extract and set specific command params based on command_type
    const CommandParamsVariant &paramsVariant = message.getParams();
    if (std::holds_alternative<NodeWithColorParams>(paramsVariant)) {
        const NodeWithColorParams &fillParams = std::get<NodeWithColorParams>(paramsVariant);
        proto_msg.params.fill_node_params.node_id = fillParams.nodeId;
        std::cout << "SERIALIZED node_id " << proto_msg.params.fill_node_params.node_id << std::endl;
        proto_msg.params.fill_node_params.color = fillParams.color; // Assuming direct assignment is possible
        proto_msg.which_params = ClusterMessageProto_fill_node_params_tag; // Update this line according to your generated code.
    }

    std::cout << "cid at proto level: " << proto_msg.cluster_id << std::endl;
    uint8_t buffer[256]; // Adjust the buffer size as necessary
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

    if (!pb_encode(&stream, ClusterMessageProto_fields, &proto_msg)) {
        std::cout << " ENCODE ERRROR!!! " << std::endl;
    } else {
        std::cout << " ENCODED NICELY!!! " << stream.bytes_written << std::endl;
    }
    return std::vector<uint8_t>(buffer, buffer + stream.bytes_written);
}

ClusterMessage deserializeClusterMessage(const std::vector<uint8_t> &buffer) {
    ClusterMessageProto proto_msg = ClusterMessageProto_init_zero;
    pb_istream_t stream = pb_istream_from_buffer(buffer.data(), buffer.size());

    if (!pb_decode(&stream, ClusterMessageProto_fields, &proto_msg)) {
        std::cout << "DECODING ERRRRRRORRRR" << std::endl;
        // Handle decoding error
    }

    CommandParamsVariant params;
    // Handle the specific command parameters based on the command type
    if (proto_msg.command_type == CommandTypeProto::CommandTypeProto_FILL_NODE) {
        std::cout << "proto matches type" << std::endl;
        // error confirmed HERE:
        std::cout << "Deserialized node_id " << proto_msg.params.fill_node_params.node_id << std::endl;
        NodeWithColorParams fillParams;
        fillParams.nodeId = proto_msg.params.fill_node_params.node_id;
        fillParams.color = proto_msg.params.fill_node_params.color;
        params = fillParams;
    } else {
        std::cout << "proto fails to match type" << std::endl;
    }

    std::cout << "proto cluster id: " << proto_msg.cluster_id << std::endl;

    if (std::holds_alternative<NodeWithColorParams>(params)) {
        // Step 3: Extract NodeWithColorParams
        NodeWithColorParams castedParams = std::get<NodeWithColorParams>(params);
        std::cout << "proto node_id: " << castedParams.nodeId << std::endl;
    }
    ClusterMessage message(proto_msg.cluster_id, static_cast<ClusterCommandType>(proto_msg.command_type), params);

    return message;
}
