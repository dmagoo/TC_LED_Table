// ClusterManager.h
#ifndef CLUSTERCOMMANDS_H
#define CLUSTERCOMMANDS_H

#include "Cluster.h"
#include "LedTableTypes.h"
#include <iomanip>
#include <iostream>
#include <variant>

/***
 * adding a command
 * 1. add command type to ClusterCommandType enum
 * 2. if parameters signature doesn't exist, create a struct for it
 * 2a. Add it to CommandParamsVariant
 * 3. if return type doesn't exist, create an interface for it
 * 4. create the Command class, extending the interface
 * 5. Add any params / classes to src/core/proto/ClusterMessages.proto
 * 6. Follow instructions in ClusterMessages.proto to update the code via nanopb
 * 7. In LedTableApi.ccp add appropriate pass-through methods if the command is
 *    Is accessible to the api user. Perhaps some ClusterCommands are internal to
 *    The Api?
 * 8. Add case to executeMessageCommand in ClusterMessage.cpp
 * 9. register types in Message serialize/desirialize
 ***/

enum class ClusterCommandType {
    FillNode = 0,       // Matches FILL_NODE
    BlitNode = 1,       // Matches BLIT_NODE
    FillBuffer = 2,     // Matches FILL_BUFFER (Note the skip to align with the protobuf enum)
    BlitBuffer = 3,     // Matches BLIT_BUFFER
    SetNodePixel = 4,   // Matches SET_NODE_PIXEL
    QueueNodePixel = 5, // Matches QUEUE_NODE_PIXEL
    DequeueNodePixel = 6 // Matches DEQUEUE_NODE_PIXEL
};

// parms structured for use in messaging
struct NodeWithColorParams {
    int nodeId;
    WRGB color;
};

struct NodeWithPixelIndexParams {
    int nodeId;
    int pixelIndex;
    WRGB color;
};

struct BlitNodeParams {
    int nodeId;
    std::vector<WRGB> colors;
    WRGB padColor;
};

struct BlitBufferParams {
    std::vector<WRGB> colors;
    WRGB padColor;
};

struct FillBufferParams {
    WRGB color;
};

using CommandParamsVariant = std::variant<
    NodeWithColorParams,
    BlitNodeParams,
    NodeWithPixelIndexParams,
    FillBufferParams,
    BlitBufferParams
>;

// Command interfaces
class ClusterCommandReturningVoid {
public:
    virtual ~ClusterCommandReturningVoid() {}
    virtual void execute(Cluster &cluster) = 0;
};

class ClusterCommandReturningColor {
public:
    virtual ~ClusterCommandReturningColor() {}
    virtual WRGB execute(Cluster &cluster) = 0;
};

class FillNodeCommand : public ClusterCommandReturningVoid {
    int nodeId;
    WRGB color;

public:
    FillNodeCommand(int nodeId, WRGB color)
        : nodeId(nodeId), color(color) {}

    void execute(Cluster &cluster) override {
#ifdef DEBUG
        std::cout << "filling node: " << nodeId << " with color: 0x"
                  << std::hex << static_cast<WRGB>(color) << std::endl;
        std::cout << std::dec;
#endif
        cluster.fillNode(nodeId, color);
    }

    static ClusterCommandType getType() {
        return ClusterCommandType::FillNode;
    }

    NodeWithColorParams getParams() const {
        return NodeWithColorParams{nodeId, color};
    }
};

class BlitNodeCommand : public ClusterCommandReturningVoid {
    int nodeId;
    std::vector<WRGB> colors; // Vector passed by value
    WRGB padColor;

public:
    BlitNodeCommand(int nodeId, std::vector<WRGB> colors, WRGB padColor)
        : nodeId(nodeId), colors(std::move(colors)), padColor(padColor) {}

    void execute(Cluster &cluster) override {
        cluster.fillNode(nodeId, colors, padColor);
    }

    static ClusterCommandType getType() {
        return ClusterCommandType::BlitNode;
    }

    BlitNodeParams getParams() const {
        return BlitNodeParams{nodeId, colors, padColor};
    }
};

class FillBufferCommand : public ClusterCommandReturningVoid {
    WRGB color;

public:
    FillBufferCommand(WRGB color)
        : color(color) {}

    void execute(Cluster &cluster) override {
        cluster.fillBuffer(color);
    }

    static ClusterCommandType getType() {
        return ClusterCommandType::FillBuffer;
    }

    FillBufferParams getParams() const {
        return FillBufferParams{color};
    }
};

class BlitBufferCommand : public ClusterCommandReturningVoid {
    std::vector<WRGB> colors; // Vector passed by value
    WRGB padColor;

public:
    void execute(Cluster &cluster) override {
        cluster.fillBuffer(colors, padColor);
    }

    BlitBufferCommand(std::vector<WRGB> colors, WRGB padColor)
        : colors(std::move(colors)), padColor(padColor) {}

    static ClusterCommandType getType() {
        return ClusterCommandType::BlitBuffer;
    }

    BlitBufferParams getParams() const {
        return BlitBufferParams{colors, padColor};
    }
};

class SetNodePixelCommand : public ClusterCommandReturningVoid {
    int nodeId;
    int pixelIndex;
    WRGB color;

public:
    SetNodePixelCommand(int nodeId, int pixelIndex, WRGB color)
        : nodeId(nodeId), pixelIndex(pixelIndex), color(color) {}

    void execute(Cluster &cluster) override {
        cluster.setNodePixel(nodeId, pixelIndex, color);
    }

    static ClusterCommandType getType() {
        return ClusterCommandType::SetNodePixel;
    }

    NodeWithPixelIndexParams getParams() const {
        return NodeWithPixelIndexParams{nodeId, pixelIndex, color};
    }
};

// QueueNodePixelCommand
class QueueNodePixelCommand : public ClusterCommandReturningColor {
    int nodeId;
    WRGB color;

public:
    QueueNodePixelCommand(int nodeId, WRGB color)
        : nodeId(nodeId), color(color) {}

    WRGB execute(Cluster &cluster) override {
        // Implementation to queue the node color
        return cluster.queueNodeColor(nodeId, color);
    }

    static ClusterCommandType getType() {
        return ClusterCommandType::QueueNodePixel;
    }

    NodeWithColorParams getParams() const {
        return NodeWithColorParams{nodeId, color};
    }
};

// DequeueNodePixelCommand
class DequeueNodePixelCommand : public ClusterCommandReturningColor {
    int nodeId;
    WRGB color;

public:
    DequeueNodePixelCommand(int nodeId, WRGB color)
        : nodeId(nodeId), color(color) {}

    WRGB execute(Cluster &cluster) override {
        return cluster.dequeueNodeColor(nodeId, color);
    }

    static ClusterCommandType getType() {
        return ClusterCommandType::DequeueNodePixel;
    }

    NodeWithColorParams getParams() const {
        return NodeWithColorParams{nodeId, color};
    }
};

#endif // CLUSTERCOMMANDS_H