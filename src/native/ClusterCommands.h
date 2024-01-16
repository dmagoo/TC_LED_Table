// ClusterManager.h
#ifndef CLUSTERCOMMANDS_H
#define CLUSTERCOMMANDS_H

#include "Cluster.h"


// Command interfaces
class ClusterCommandReturningVoid {
public:
    virtual ~ClusterCommandReturningVoid() {}
    virtual void execute(Cluster& cluster) = 0;
};

class ClusterCommandReturningColor {
public:
    virtual ~ClusterCommandReturningColor() {}
    virtual int32_t execute(Cluster& cluster) = 0;
};

class FillNodeCommand : public ClusterCommandReturningColor {
    int nodeId;
    int32_t color;
public:
    FillNodeCommand(int nodeId, int32_t color)
        : nodeId(nodeId), color(color) {}

    int32_t execute(Cluster& cluster) override {
        // Implementation to set the node color
        return 0;
    }
};

class SetNodePixelCommand : public ClusterCommandReturningVoid {
    int nodeId;
    int32_t color;
public:
    SetNodePixelCommand(int nodeId, int32_t color)
        : nodeId(nodeId), color(color) {}

    void execute(Cluster& cluster) override {
        // Implementation to set the node color
    }
};

// QueueNodePixelCommand
class QueueNodePixelCommand : public ClusterCommandReturningColor {
    int nodeId;
    int32_t color;
public:
    QueueNodePixelCommand(int nodeId, int32_t color)
        : nodeId(nodeId), color(color) {}

    int32_t execute(Cluster& cluster) override {
        // Implementation to queue the node color
        return 0;
    }
};

// DequeueNodePixelCommand
class DequeueNodePixelCommand : public ClusterCommandReturningColor {
    int nodeId;
public:
    DequeueNodePixelCommand(int nodeId)
        : nodeId(nodeId) {}

    int32_t execute(Cluster& cluster) override {
        // Implementation to dequeue the node color
        return 0;
    }
};

#endif // CLUSTERCOMMANDS_H