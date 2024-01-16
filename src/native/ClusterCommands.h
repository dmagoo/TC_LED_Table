// ClusterManager.h
#ifndef CLUSTERCOMMANDS_H
#define CLUSTERCOMMANDS_H

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
    virtual int32_t execute(Cluster &cluster) = 0;
};

class FillNodeCommand : public ClusterCommandReturningVoid {
    int nodeId;
    int32_t color;

public:
    FillNodeCommand(int nodeId, int32_t color)
        : nodeId(nodeId), color(color) {}

    void execute(Cluster &cluster) override {
        std::cout << "filling node: " << nodeId << " with color: 0x"
                  << std::hex << static_cast<uint32_t>(color) << std::endl;
        std::cout << std::dec;
        cluster.fillNode(nodeId, color);
    }
};

class SetNodePixelCommand : public ClusterCommandReturningVoid {
    int nodeId;
    int pixelIndex;
    int32_t color;

public:
    SetNodePixelCommand(int nodeId, int pixelIndex, int32_t color)
        : nodeId(nodeId), pixelIndex(pixelIndex), color(color) {}

    void execute(Cluster &cluster) override {
        cluster.setNodePixel(nodeId, pixelIndex, color);
    }
};

// QueueNodePixelCommand
class QueueNodePixelCommand : public ClusterCommandReturningColor {
    int nodeId;
    int32_t color;

public:
    QueueNodePixelCommand(int nodeId, int32_t color)
        : nodeId(nodeId), color(color) {}

    int32_t execute(Cluster &cluster) override {
        // Implementation to queue the node color
        return cluster.queueNodeColor(nodeId, color);
    }
};

// DequeueNodePixelCommand
class DequeueNodePixelCommand : public ClusterCommandReturningColor {
    int nodeId;
    int32_t color;
public:
    DequeueNodePixelCommand(int nodeId, int32_t color)
        : nodeId(nodeId), color(color) {}

    int32_t execute(Cluster &cluster) override {
        return cluster.dequeueNodeColor(nodeId, color);
    }
};

#endif // CLUSTERCOMMANDS_H