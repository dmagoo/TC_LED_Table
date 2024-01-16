#include "LedTableApi.h"

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
template int32_t LedTableApi::performClusterOperationReturningColor<DequeueNodePixelCommand>(int);

// Template specialization for RingCoordinate
template<>
int LedTableApi::convertToNodeId<RingCoordinate>(const RingCoordinate& coordinate) {
    // Implement the actual conversion logic here
    // Return the nodeId corresponding to the RingCoordinate
    return 0;
}

// Template specialization for Cartesian2dCoordinate
template<>
int LedTableApi::convertToNodeId<Cartesian2dCoordinate>(const Cartesian2dCoordinate& coordinate) {
    // Implement the actual conversion logic here
    // Return the nodeId corresponding to the Cartesian2dCoordinate
    return 0;
}

// Template specialization for Cartesian2dCoordinate
template<>
int LedTableApi::convertToNodeId<CubeCoordinate>(const CubeCoordinate& coordinate) {
    // Implement the actual conversion logic here
    // Return the nodeId corresponding to the CubeCoordinate
    return 0;
}


// Similar specializations for CubeCoordinate and other types...

// Implementations of the public API methods using the template function
void LedTableApi::fillNode(int nodeId, int32_t color) {
    performClusterOperationReturningVoid<FillNodeCommand>(nodeId, color);
}

void LedTableApi::fillNode(RingCoordinate coordinate, int32_t color) {
    int nodeId = convertToNodeId(coordinate);
    performClusterOperationReturningVoid<FillNodeCommand>(nodeId, color);
}

void LedTableApi::fillNode(CubeCoordinate coordinate, int32_t color) {
    int nodeId = convertToNodeId(coordinate);
    performClusterOperationReturningVoid<FillNodeCommand>(nodeId, color);
}

void LedTableApi::fillNode(Cartesian2dCoordinate coordinate, int32_t color) {
    int nodeId = convertToNodeId(coordinate);
    performClusterOperationReturningVoid<FillNodeCommand>(nodeId, color);
}

// Implementations of the public API methods using the template function
void LedTableApi::setNodePixel(int nodeId, int32_t color) {
    performClusterOperationReturningVoid<SetNodePixelCommand>(nodeId, color);
}

void LedTableApi::setNodePixel(RingCoordinate coordinate, int32_t color) {
    int nodeId = convertToNodeId(coordinate);
    performClusterOperationReturningVoid<SetNodePixelCommand>(nodeId, color);
}

void LedTableApi::setNodePixel(CubeCoordinate coordinate, int32_t color) {
    int nodeId = convertToNodeId(coordinate);
    performClusterOperationReturningVoid<SetNodePixelCommand>(nodeId, color);
}

void LedTableApi::setNodePixel(Cartesian2dCoordinate coordinate, int32_t color) {
    int nodeId = convertToNodeId(coordinate);
    performClusterOperationReturningVoid<SetNodePixelCommand>(nodeId, color);
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
int32_t LedTableApi::dequeueNodePixel(int nodeId) {
    return performClusterOperationReturningColor<DequeueNodePixelCommand>(nodeId);
}

int32_t LedTableApi::dequeueNodePixel(RingCoordinate coordinate) {
    int nodeId = convertToNodeId(coordinate);
    return performClusterOperationReturningColor<DequeueNodePixelCommand>(nodeId);
}

int32_t LedTableApi::dequeueNodePixel(CubeCoordinate coordinate) {
    int nodeId = convertToNodeId(coordinate);
    return performClusterOperationReturningColor<DequeueNodePixelCommand>(nodeId);
}

int32_t LedTableApi::dequeueNodePixel(Cartesian2dCoordinate coordinate) {
    int nodeId = convertToNodeId(coordinate);
    return performClusterOperationReturningColor<DequeueNodePixelCommand>(nodeId);
}

void LedTableApi::setSuppressMessages(bool newValue) {}
void LedTableApi::setSuppressRefresh(bool newValue) {}
