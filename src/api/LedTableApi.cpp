#include "LedTableApi.h"
#include <iostream>

template<typename CommandType, typename... Args>
void LedTableApi::performClusterOperationReturningVoid(int nodeId, Args... args) {
    int clusterId = clusterManager.getClusterIdFromNodeId(nodeId);
    const Cluster* clusterPtr = clusterManager.getClusterById(clusterId);
    
    if (clusterPtr) {
        Cluster& cluster = *const_cast<Cluster*>(clusterPtr); // Cast to non-const reference
        CommandType command(nodeId, args...);
        command.execute(cluster);
    } else {
        // Handle the case where the cluster is not found
    }
}

template<typename CommandType, typename... Args>
int32_t LedTableApi::performClusterOperationReturningColor(int nodeId, Args... args) {
    int clusterId = clusterManager.getClusterIdFromNodeId(nodeId);
    const Cluster* clusterPtr = clusterManager.getClusterById(clusterId);
    
    if (clusterPtr) {
        Cluster& cluster = *const_cast<Cluster*>(clusterPtr); // Cast to non-const reference
        CommandType command(nodeId, args...);
        return command.execute(cluster);
    } else {
        // Handle the case where the cluster is not found
        return 0;
    }
}


// Explicit template instantiations for all the command types you use
template void LedTableApi::performClusterOperationReturningVoid<FillNodeCommand, int32_t>(int, int32_t);
template int32_t LedTableApi::performClusterOperationReturningColor<QueueNodePixelCommand, int32_t>(int, int32_t);
template int32_t LedTableApi::performClusterOperationReturningColor<DequeueNodePixelCommand>(int, int32_t);

// Template specialization for RingCoordinate
template<>
int LedTableApi::convertToNodeId<RingCoordinate>(const RingCoordinate& coordinate) {
    return clusterManager.getNodeId(coordinate);
}

// Template specialization for Cartesian2dCoordinate
template<>
int LedTableApi::convertToNodeId<Cartesian2dCoordinate>(const Cartesian2dCoordinate& coordinate) {
    return clusterManager.getNodeId(coordinate);
}

// Template specialization for CubeCoordinate
template<>
int LedTableApi::convertToNodeId<CubeCoordinate>(const CubeCoordinate& coordinate) {
    return clusterManager.getNodeId(coordinate);
}


// Similar specializations for CubeCoordinate and other types...

// Implementations of the public API methods using the template function
void LedTableApi::fillNode(int nodeId, int32_t color) {
std::cout << "in root fill with node: " << nodeId << std::endl;
    performClusterOperationReturningVoid<FillNodeCommand>(nodeId, color);
}

void LedTableApi::fillNode(RingCoordinate coordinate, int32_t color) {
    int nodeId = convertToNodeId(coordinate);
std::cout << "in ring fill with node: " << nodeId << std::endl;
    performClusterOperationReturningVoid<FillNodeCommand>(nodeId, color);
}

void LedTableApi::fillNode(CubeCoordinate coordinate, int32_t color) {
    int nodeId = convertToNodeId(coordinate);
std::cout << "in cube fill with node: " << nodeId << std::endl;
    performClusterOperationReturningVoid<FillNodeCommand>(nodeId, color);
}

void LedTableApi::fillNode(Cartesian2dCoordinate coordinate, int32_t color) {
    int nodeId = convertToNodeId(coordinate);
std::cout << "in cart2d fill with node: " << nodeId << std::endl;
    performClusterOperationReturningVoid<FillNodeCommand>(nodeId, color);
}

// Implementations of the public API methods using the template function
void LedTableApi::setNodePixel(int nodeId, int pixelIndex, int32_t color) {
    performClusterOperationReturningVoid<SetNodePixelCommand>(nodeId, pixelIndex, color);
}

void LedTableApi::setNodePixel(RingCoordinate coordinate, int pixelIndex, int32_t color) {
    int nodeId = convertToNodeId(coordinate);
    performClusterOperationReturningVoid<SetNodePixelCommand>(nodeId, pixelIndex, color);
}

void LedTableApi::setNodePixel(CubeCoordinate coordinate, int pixelIndex, int32_t color) {
    int nodeId = convertToNodeId(coordinate);
    performClusterOperationReturningVoid<SetNodePixelCommand>(nodeId, pixelIndex, color);
}

void LedTableApi::setNodePixel(Cartesian2dCoordinate coordinate, int pixelIndex, int32_t color) {
    int nodeId = convertToNodeId(coordinate);
    performClusterOperationReturningVoid<SetNodePixelCommand>(nodeId, pixelIndex, color);
}

// Implementations of the public API methods using the template function
int32_t LedTableApi::queueNodePixel(int nodeId, int32_t color) {
    return performClusterOperationReturningColor<QueueNodePixelCommand>(nodeId, color);
}

int32_t LedTableApi::queueNodePixel(RingCoordinate coordinate, int32_t color) {
    int nodeId = convertToNodeId(coordinate);
    return performClusterOperationReturningColor<QueueNodePixelCommand>(nodeId, color);
}

int32_t LedTableApi::queueNodePixel(CubeCoordinate coordinate, int32_t color) {
    int nodeId = convertToNodeId(coordinate);
    return performClusterOperationReturningColor<QueueNodePixelCommand>(nodeId, color);
}

int32_t LedTableApi::queueNodePixel(Cartesian2dCoordinate coordinate, int32_t color) {
    int nodeId = convertToNodeId(coordinate);
    return performClusterOperationReturningColor<QueueNodePixelCommand>(nodeId, color);
}

// Implementations of the public API methods using the template function
int32_t LedTableApi::dequeueNodePixel(int nodeId, int32_t color) {
    return performClusterOperationReturningColor<DequeueNodePixelCommand>(nodeId, color);
}

int32_t LedTableApi::dequeueNodePixel(RingCoordinate coordinate, int32_t color) {
    int nodeId = convertToNodeId(coordinate);
    return performClusterOperationReturningColor<DequeueNodePixelCommand>(nodeId, color);
}

int32_t LedTableApi::dequeueNodePixel(CubeCoordinate coordinate, int32_t color) {
    int nodeId = convertToNodeId(coordinate);
    return performClusterOperationReturningColor<DequeueNodePixelCommand>(nodeId, color);
}

int32_t LedTableApi::dequeueNodePixel(Cartesian2dCoordinate coordinate, int32_t color) {
    int nodeId = convertToNodeId(coordinate);
    return performClusterOperationReturningColor<DequeueNodePixelCommand>(nodeId, color);
}

void LedTableApi::setSuppressMessages(bool newValue) {}
void LedTableApi::setSuppressRefresh(bool newValue) {}
