#include "ClusterMessage.h"
#include "ClusterCommands.h"
#include "core/proto/ClusterMessagesProto.pb.h"
#include <iostream>
#include <variant>
#include <vector>

int ClusterMessage::getClusterId() const {
    return clusterId;
}

ClusterCommandType ClusterMessage::getCommandType() const {
    return commandType;
}

CommandParamsVariant ClusterMessage::getParams() const {
    return params;
}

/*

ClusterMessage createFillNodeMessage(int clusterId, int nodeId, int color) {
    NodeWithColorParams params;
    params.nodeId = nodeId;
    params.color = color;
    ClusterMessage clusterMessage(clusterId, ClusterCommandType::FillNode, params);
    return clusterMessage;
}

ClusterMessage createBlitNodeMessage(int clusterId, int nodeId, std::vector<WRGB> colors, WRGB padColor) {
    BlitNodeParams params;
    params.nodeId = nodeId;
    params.colors = std::move(colors);
    params.padColor = padColor;
    ClusterMessage clusterMessage(clusterId, ClusterCommandType::BlitNode, params);
    return clusterMessage;
}

ClusterMessage createSetNodePixelMessage(int clusterId, int nodeId, int color) {
    NodeWithPixelIndexParams params;
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
*/

/**
 * Encodes a vector of WRGB colors into a protobuf stream as a repeated field.
 * This function is typically used as a callback in the protobuf encoding process,
 * where it serializes each color from the vector into the protobuf message format.
 *
 * @param stream The protobuf output stream where the encoded data is written.
 * @param field The protobuf field being encoded, providing context for the encoding.
 * @param arg A pointer to the user-provided argument, expected to be a `std::vector<WRGB>*`
 *            containing the colors to encode.
 * @return True if encoding succeeds, false otherwise.
 */
bool serializeColors(pb_ostream_t *stream, const pb_field_t *field, void *const *arg) {
    const std::vector<uint32_t> *colors = reinterpret_cast<const std::vector<uint32_t> *>(*arg);

    for (const uint32_t color : *colors) {
        // First, encode the tag for the field. This indicates the field number and wire type.
        if (!pb_encode_tag_for_field(stream, field)) {
            std::cout << "Failed to encode tag for field." << std::endl;
            return false; // Return false on encode failure
        }

        // Then, encode the color value as a fixed-size 32-bit integer.
        if (!pb_encode_varint(stream, color)) {
            std::cout << "Failed to encode color value." << std::endl;
            return false; // Return false on encode failure
        }
    }
    return true; // Return true on success
}

bool deserializeColorCallback(pb_istream_t *stream, const pb_field_t *field, void **arg) {
    uint32_t color;

    if (!pb_decode_varint32(stream, &color)) {
        std::cout << "Failed deserializing color" << std::endl;
        return false; // Failed to decode color
    }

    std::vector<uint32_t> *colors = reinterpret_cast<std::vector<uint32_t> *>(*arg);
    colors->push_back(color);
    return true;
}

std::vector<uint8_t> serializeClusterMessage(const ClusterMessage &message) {
    ClusterMessageProto proto_msg = ClusterMessageProto_init_zero;
    proto_msg.cluster_id = message.getClusterId();
    proto_msg.command_type = static_cast<CommandTypeProto>(message.getCommandType());

    // Extract and set specific command params based params type
    const CommandParamsVariant &paramsVariant = message.getParams();
    if (std::holds_alternative<NodeWithColorParams>(paramsVariant)) {
        const NodeWithColorParams &commandParams = std::get<NodeWithColorParams>(paramsVariant);
        proto_msg.param_node_id = commandParams.nodeId;
        proto_msg.param_color = commandParams.color;
    } else if (std::holds_alternative<BlitNodeParams>(paramsVariant)) {
        const BlitNodeParams &commandParams = std::get<BlitNodeParams>(paramsVariant);
        proto_msg.param_node_id = commandParams.nodeId;
        proto_msg.param_pad_color = commandParams.padColor;
        proto_msg.param_colors.funcs.encode = serializeColors;
        proto_msg.param_colors.arg = (void *)&commandParams.colors;
    } else if (std::holds_alternative<NodeWithPixelIndexParams>(paramsVariant)) {
        const NodeWithPixelIndexParams &commandParams = std::get<NodeWithPixelIndexParams>(paramsVariant);
        proto_msg.param_node_id = commandParams.nodeId;
        proto_msg.param_pixel_index = commandParams.pixelIndex;
        proto_msg.param_color = commandParams.color;
    } else if (std::holds_alternative<FillBufferParams>(paramsVariant)) {
        const FillBufferParams &commandParams = std::get<FillBufferParams>(paramsVariant);
        proto_msg.param_color = commandParams.color;
    } else if (std::holds_alternative<BlitBufferParams>(paramsVariant)) {
        const BlitBufferParams &commandParams = std::get<BlitBufferParams>(paramsVariant);
        proto_msg.param_pad_color = commandParams.padColor;
        proto_msg.param_colors.funcs.encode = serializeColors;
        proto_msg.param_colors.arg = (void *)&commandParams.colors;
    }

    uint8_t buffer[1024]; // Adjust the buffer size as necessary
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

    if (!pb_encode(&stream, ClusterMessageProto_fields, &proto_msg)) {
        std::cout << " Encoding error" << std::endl;
    }

    // Assume 'stream' is your pb_ostream_t used for encoding
    // std::cout << "Encoded data (" << stream.bytes_written << " bytes): ";
    // for (size_t i = 0; i < stream.bytes_written; ++i) {
    //    std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)buffer[i] << " ";
    //}
    // std::cout << std::dec << std::endl; // Switch back to decimal for any future outputs

    return std::vector<uint8_t>(buffer, buffer + stream.bytes_written);
}

ClusterMessage deserializeClusterMessage(const std::vector<uint8_t> &buffer) {
    ClusterMessageProto proto_msg = ClusterMessageProto_init_zero;

    pb_istream_t stream = pb_istream_from_buffer(buffer.data(), buffer.size());

    std::vector<uint32_t> tempColors;
    proto_msg.param_colors.arg = &tempColors;
    proto_msg.param_colors.funcs.decode = deserializeColorCallback;
    if (!pb_decode(&stream, ClusterMessageProto_fields, &proto_msg)) {
        std::cerr << "DECODING ERROR: " << PB_GET_ERROR(&stream) << std::endl;
        throw std::runtime_error("Decoding failed for ClusterMessageProto");
        // Handle decoding error
    }

    CommandParamsVariant params;
    switch (proto_msg.command_type) {
    case CommandTypeProto::CommandTypeProto_FILL_NODE: {
        NodeWithColorParams commandParams;
        commandParams.nodeId = proto_msg.param_node_id;
        commandParams.color = proto_msg.param_color;
        params = commandParams;
        break;
    }
    case CommandTypeProto::CommandTypeProto_BLIT_NODE: {
        BlitNodeParams commandParams;
        commandParams.nodeId = proto_msg.param_node_id;
        commandParams.padColor = proto_msg.param_pad_color;
        commandParams.colors = std::move(tempColors);
        params = commandParams;
        break;
    }
    case CommandTypeProto::CommandTypeProto_FILL_BUFFER: {
        FillBufferParams commandParams;
        commandParams.color = proto_msg.param_color;
        params = commandParams;
        break;
    }
    case CommandTypeProto::CommandTypeProto_BLIT_BUFFER: {
        BlitBufferParams commandParams;
        commandParams.padColor = proto_msg.param_pad_color;
        commandParams.colors = std::move(tempColors);
        params = commandParams;
        break;
    }
    case CommandTypeProto::CommandTypeProto_SET_NODE_PIXEL: {
        NodeWithPixelIndexParams commandParams;
        commandParams.nodeId = proto_msg.param_node_id;
        commandParams.pixelIndex = proto_msg.param_pixel_index;
        commandParams.color = proto_msg.param_color;
        params = commandParams;
        break;
    }
    case CommandTypeProto::CommandTypeProto_QUEUE_NODE_PIXEL: {
        NodeWithPixelIndexParams commandParams;
        commandParams.nodeId = proto_msg.param_node_id;
        commandParams.color = proto_msg.param_color;
        params = commandParams;
        break;
    }
    case CommandTypeProto::CommandTypeProto_DEQUEUE_NODE_PIXEL: {
        NodeWithPixelIndexParams commandParams;
        commandParams.nodeId = proto_msg.param_node_id;
        commandParams.color = proto_msg.param_color;
        params = commandParams;
        break;
    }
    default:
        std::cout << "Proto command fails to match type" << std::endl;
        break;
    }

    if (std::holds_alternative<NodeWithColorParams>(params)) {
        // Step 3: Extract NodeWithColorParams
        NodeWithColorParams castedParams = std::get<NodeWithColorParams>(params);
    }

    ClusterMessage message(proto_msg.cluster_id, static_cast<ClusterCommandType>(proto_msg.command_type), params);

    return message;
}

// Function to execute the command contained within a ClusterMessage
void executeMessageCommand(const ClusterMessage &clusterMessage, Cluster &cluster) {
    switch (clusterMessage.getCommandType()) {
    case ClusterCommandType::FillNode: {
        auto params = std::get<NodeWithColorParams>(clusterMessage.getParams());
        FillNodeCommand command(params.nodeId, params.color);
        command.execute(cluster);
        break;
    }
    case ClusterCommandType::BlitNode: {
        auto params = std::get<BlitNodeParams>(clusterMessage.getParams());
        BlitNodeCommand command(params.nodeId, params.colors, params.padColor);
        command.execute(cluster);
        break;
    }
    case ClusterCommandType::SetNodePixel: {
        auto params = std::get<NodeWithPixelIndexParams>(clusterMessage.getParams());
        SetNodePixelCommand command(params.nodeId, params.pixelIndex, params.color);
        command.execute(cluster);
        break;
    }
    case ClusterCommandType::QueueNodePixel: {
        auto params = std::get<NodeWithColorParams>(clusterMessage.getParams());
        QueueNodePixelCommand command(params.nodeId, params.color);
        // Assuming you have a mechanism to handle or ignore the return value
        command.execute(cluster);
        break;
    }
    case ClusterCommandType::DequeueNodePixel: {
        auto params = std::get<NodeWithColorParams>(clusterMessage.getParams());
        DequeueNodePixelCommand command(params.nodeId, params.color);
        // Assuming you have a mechanism to handle or ignore the return value
        command.execute(cluster);
        break;
    }
    case ClusterCommandType::FillBuffer: {
        auto params = std::get<FillBufferParams>(clusterMessage.getParams());
        FillBufferCommand command(params.color);
        command.execute(cluster);
        break;
    }
    case ClusterCommandType::BlitBuffer: {
        auto params = std::get<BlitBufferParams>(clusterMessage.getParams());
        BlitBufferCommand command(params.colors, params.padColor);
        command.execute(cluster);
        break;
    }
    default:
        // Handle unknown command type, perhaps log an error
        break;
    }
}
