#ifndef LEDTABLEAPI_H
#define LEDTABLEAPI_H

#include "native/ClusterManager.h"
#include "native/coordinates/RingCoordinate.h"
#include "native/coordinates/CubeCoordinate.h"
#include "native/coordinates/Cartesian2dCoordinate.h"
#include "native/ClusterCommands.h"

class LedTableApi {
    bool suppressMessages = false;
    bool suppressRefresh = false;

public:
    LedTableApi(ClusterManager& clusterManager) : clusterManager(clusterManager) {}

    void setSuppressMessages(bool newValue);

    void setSuppressRefresh(bool newValue);

    void fillNode(int nodeId, int32_t color);
    void fillNode(RingCoordinate coordinate, int32_t color);
    void fillNode(Cartesian2dCoordinate coordinate, int32_t color);
    void fillNode(CubeCoordinate coordinate, int32_t color);

    void setNodePixel(int nodeId, int pixelIndex, int32_t color);
    void setNodePixel(RingCoordinate coordinate, int pixelIndex, int32_t color);
    void setNodePixel(Cartesian2dCoordinate coordinate, int pixelIndex, int32_t color);
    void setNodePixel(CubeCoordinate coordinate, int pixelIndex, int32_t color);

    int32_t queueNodePixel(int nodeId, int32_t color);
    int32_t queueNodePixel(RingCoordinate coordinate, int32_t color);
    int32_t queueNodePixel(Cartesian2dCoordinate coordinate, int32_t color);
    int32_t queueNodePixel(CubeCoordinate coordinate, int32_t color);

    int32_t dequeueNodePixel(int nodeId, int32_t color);
    int32_t dequeueNodePixel(RingCoordinate coordinate, int32_t color);
    int32_t dequeueNodePixel(Cartesian2dCoordinate coordinate, int32_t color);
    int32_t dequeueNodePixel(CubeCoordinate coordinate, int32_t color);

private:
    ClusterManager& clusterManager;

    template<typename CoordinateType>
    int convertToNodeId(const CoordinateType& coordinate);

    template<typename CommandType, typename... Args>
    void performClusterOperationReturningVoid(int nodeId, Args... args);

    template<typename CommandType, typename... Args>
    int32_t performClusterOperationReturningColor(int nodeId, Args... args);

};

#endif // LEDTABLEAPI_H
