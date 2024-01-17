// ClusterManager.h
#ifndef CLUSTERCOMMANDS_H
#define CLUSTERCOMMANDS_H

#include "LedTableTypes.h"
#include "Cluster.h"
#include <iomanip>
#include <iostream>

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
};

class BlitNodeCommand : public ClusterCommandReturningVoid {
    int nodeId;
    std::vector<RGBW> colors; // Vector passed by value
    RGBW padColor;
public:
    BlitNodeCommand(int nodeId, std::vector<RGBW> colors, RGBW padColor)
        : nodeId(nodeId), colors(std::move(colors)), padColor(padColor) {}

    void execute(Cluster& cluster) override {
        cluster.fillNode(nodeId, colors, padColor);
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
};

#endif // CLUSTERCOMMANDS_H