#ifndef LEDTABLEAPI_H
#define LEDTABLEAPI_H

#include "core/ClusterManager.h"
#include "core/coordinates/RingCoordinate.h"
#include "core/coordinates/CubeCoordinate.h"
#include "core/coordinates/Cartesian2dCoordinate.h"
#include "core/ClusterCommands.h"

class LedTableApi {
    bool suppressMessages = false;
    bool suppressRefresh = false;

public:
    LedTableApi(ClusterManager& clusterManager) : clusterManager(clusterManager) {}

    void setSuppressMessages(bool newValue);

    void setSuppressRefresh(bool newValue);

    void fillNode(int nodeId, RGBW color);
    void fillNode(RingCoordinate coordinate, RGBW color);
    void fillNode(Cartesian2dCoordinate coordinate, RGBW color);
    void fillNode(CubeCoordinate coordinate, RGBW color);

    void fillNode(int nodeId, const std::vector<RGBW>& colors, RGBW padcolor);
    void fillNode(RingCoordinate coordinate, const std::vector<RGBW>& colors, RGBW padcolor);
    void fillNode(Cartesian2dCoordinate coordinate, const std::vector<RGBW>& colors, RGBW padcolor);
    void fillNode(CubeCoordinate coordinate, const std::vector<RGBW>& colors, RGBW padcolor);

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

private:
    ClusterManager& clusterManager;

    template<typename CoordinateType>
    int convertToNodeId(const CoordinateType& coordinate);

    template<typename CommandType, typename... Args>
    void performClusterOperationReturningVoid(int nodeId, Args... args);

    template<typename CommandType, typename... Args>
    RGBW performClusterOperationReturningColor(int nodeId, Args... args);

};

#endif // LEDTABLEAPI_H
