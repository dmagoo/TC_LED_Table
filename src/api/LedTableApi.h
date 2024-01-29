#ifndef LEDTABLEAPI_H
#define LEDTABLEAPI_H

#include "core/ClusterCommands.h"
#include "core/ClusterManager.h"
#include "core/ClusterMessageManager.h"
#include "core/coordinates/Cartesian2dCoordinate.h"
#include "core/coordinates/CubeCoordinate.h"
#include "core/coordinates/RingCoordinate.h"

class LedTableApi {

private:
    ClusterManager &clusterManager;
    ClusterMessageManager *clusterMessageManager = nullptr;
    bool suppressMessages = false;
    bool suppressRefresh = false;

    template <typename CoordinateType>
    int convertToNodeId(const CoordinateType &coordinate);

    template <typename CommandType, typename... Args>
    void performClusterOperationReturningVoid(int nodeId, Args... args);

    template <typename CommandType, typename... Args>
    RGBW performClusterOperationReturningColor(int nodeId, Args... args);

public:
    // if (clusterMessageManager) {
    //     // Messaging is enabled, so use clusterMessageManager for operations
    // } else {
    //     // Messaging is disabled; handle accordingly
    // }
    //  OLDLedTableApi(ClusterManager& clusterManager,ClusterMessageManager& clusterMessageManager) : clusterManager(clusterManager), clusterMessageManager(clusterMessageManager) {}
    // LedTableApi(ClusterManager& clusterManager) : clusterManager(clusterManager) {}
    explicit LedTableApi(ClusterManager &clusterManager)
        : clusterManager(clusterManager), suppressMessages(true) { // Messaging is suppressed by default
    }

    LedTableApi(ClusterManager &clusterManager, ClusterMessageManager *clusterMessageManager)
        : clusterManager(clusterManager), clusterMessageManager(clusterMessageManager), suppressMessages(clusterMessageManager == nullptr) {
    }

    void setSuppressMessages(bool newValue);

    void setSuppressRefresh(bool newValue);

    void fillNode(int nodeId, RGBW color);
    void fillNode(RingCoordinate coordinate, RGBW color);
    void fillNode(Cartesian2dCoordinate coordinate, RGBW color);
    void fillNode(CubeCoordinate coordinate, RGBW color);

    void fillNode(int nodeId, const std::vector<RGBW> &colors, RGBW padcolor);
    void fillNode(RingCoordinate coordinate, const std::vector<RGBW> &colors, RGBW padcolor);
    void fillNode(Cartesian2dCoordinate coordinate, const std::vector<RGBW> &colors, RGBW padcolor);
    void fillNode(CubeCoordinate coordinate, const std::vector<RGBW> &colors, RGBW padcolor);

    void setNodePixel(int nodeId, int pixelIndex, RGBW color);
    void setNodePixel(RingCoordinate coordinate, int pixelIndex, RGBW color);
    void setNodePixel(Cartesian2dCoordinate coordinate, int pixelIndex, RGBW color);
    void setNodePixel(CubeCoordinate coordinate, int pixelIndex, RGBW color);

    RGBW queueNodePixel(int nodeId, RGBW color);
    RGBW queueNodePixel(RingCoordinate coordinate, RGBW color);
    RGBW queueNodePixel(Cartesian2dCoordinate coordinate, RGBW color);
    RGBW queueNodePixel(CubeCoordinate coordinate, RGBW color);

    RGBW dequeueNodePixel(int nodeId, RGBW color);
    RGBW dequeueNodePixel(RingCoordinate coordinate, RGBW color);
    RGBW dequeueNodePixel(Cartesian2dCoordinate coordinate, RGBW color);
    RGBW dequeueNodePixel(CubeCoordinate coordinate, RGBW color);

    // more ideas!
    // copyNode(nodeIdA, nodeIdB) // copies the buffer from a to b, using padding if not equaly sized
    // reset() // contacts all clusters, asking them to fill their buffers black, could any color fill?

    // sends fillBuffer() messages to all clusters, using the local models buffer
    // effectively updating / syncing the models. Useful if messages is off

    // refresh()
    // setSuppressMessages(bool)
    // setSuppressRefresh(bool) // lol, forget what this was!
};

#endif // LEDTABLEAPI_H
