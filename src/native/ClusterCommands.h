// ClusterManager.h
#ifndef CLUSTERCOMMANDS_H
#define CLUSTERCOMMANDS_H

#include "Cluster.h"
#include "LedTableTypes.h"
#include <iomanip>
#include <iostream>
#include <variant>

enum class ClusterCommandType {
    FillNode,
    BlitNode,
    SetNodePixel,
    QueueNodePixel,
    DequeueNodePixel
};

// parms structured for use in messaging
struct NodeWithColorParams {
    int nodeId;
    RGBW color;
};

struct NodeWithPixelIndex {
    int nodeId;
    int pixelIndex;
    RGBW color;
};

struct BlitNodeParams {
    int nodeId;
    std::vector<RGBW> colors;
    RGBW padColor;
};

using CommandParamsVariant = std::variant<NodeWithColorParams, BlitNodeParams, NodeWithPixelIndex>;

// Command interfaces
class ClusterCommandReturningVoid {
public:
    virtual ~ClusterCommandReturningVoid() {}
    virtual void execute(Cluster &cluster) = 0;
};

class ClusterCommandReturningColor {
public:
    virtual ~ClusterCommandReturningColor() {}
    virtual RGBW execute(Cluster &cluster) = 0;
};

class FillNodeCommand : public ClusterCommandReturningVoid {
    int nodeId;
    RGBW color;

public:
    FillNodeCommand(int nodeId, RGBW color)
        : nodeId(nodeId), color(color) {}

    void execute(Cluster &cluster) override {
        std::cout << "filling node: " << nodeId << " with color: 0x"
                  << std::hex << static_cast<RGBW>(color) << std::endl;
        std::cout << std::dec;
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
    std::vector<RGBW> colors; // Vector passed by value
    RGBW padColor;

public:
    BlitNodeCommand(int nodeId, std::vector<RGBW> colors, RGBW padColor)
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

class SetNodePixelCommand : public ClusterCommandReturningVoid {
    int nodeId;
    int pixelIndex;
    RGBW color;

public:
    SetNodePixelCommand(int nodeId, int pixelIndex, RGBW color)
        : nodeId(nodeId), pixelIndex(pixelIndex), color(color) {}

    void execute(Cluster &cluster) override {
        cluster.setNodePixel(nodeId, pixelIndex, color);
    }

    static ClusterCommandType getType() {
        return ClusterCommandType::SetNodePixel;
    }

    NodeWithPixelIndex getParams() const {
        return NodeWithPixelIndex{nodeId, pixelIndex, color};
    }
};

// QueueNodePixelCommand
class QueueNodePixelCommand : public ClusterCommandReturningColor {
    int nodeId;
    RGBW color;

public:
    QueueNodePixelCommand(int nodeId, RGBW color)
        : nodeId(nodeId), color(color) {}

    RGBW execute(Cluster &cluster) override {
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
    RGBW color;

public:
    DequeueNodePixelCommand(int nodeId, RGBW color)
        : nodeId(nodeId), color(color) {}

    RGBW execute(Cluster &cluster) override {
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