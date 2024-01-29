#include "LedTableApi.h"
#include "core/ClusterMessage.h"
#include <iostream>
// #include <vector>

template <typename CommandType, typename... Args>
void LedTableApi::performClusterOperationReturningVoid(int nodeId, Args... args) {
    int clusterId = clusterManager.getClusterIdFromNodeId(nodeId);
    const Cluster *clusterPtr = clusterManager.getClusterById(clusterId);

    if (clusterPtr) {
        Cluster &cluster = *const_cast<Cluster *>(clusterPtr); // Cast to non-const reference
        if constexpr (sizeof...(args) >= 2) {
            auto secondArgument = std::get<1>(std::make_tuple(args...));
        }
        //// HERE the seond argument is fine!!  Is the syntax below correct?
        CommandType command(nodeId, args...);
        // run the command against the local model
        if (!suppressMessages && clusterMessageManager != nullptr) {
            clusterMessageManager->sendClusterCommand(clusterId, command);
        }
        command.execute(cluster);
    } else {
        // Handle the case where the cluster is not found
    }
}

template <typename CommandType, typename... Args>
RGBW LedTableApi::performClusterOperationReturningColor(int nodeId, Args... args) {
    int clusterId = clusterManager.getClusterIdFromNodeId(nodeId);
    const Cluster *clusterPtr = clusterManager.getClusterById(clusterId);

    if (clusterPtr) {
        Cluster &cluster = *const_cast<Cluster *>(clusterPtr); // Cast to non-const reference
        CommandType command(nodeId, args...);
        if (!suppressMessages && clusterMessageManager != nullptr) {
            clusterMessageManager->sendClusterCommand(clusterId, command);
        }
        return command.execute(cluster);
    } else {
        // Handle the case where the cluster is not found
        return 0;
    }
}

// Explicit template instantiations for all the command types you use
template void LedTableApi::performClusterOperationReturningVoid<FillNodeCommand, RGBW>(int, RGBW);
template RGBW LedTableApi::performClusterOperationReturningColor<QueueNodePixelCommand, RGBW>(int, RGBW);
template RGBW LedTableApi::performClusterOperationReturningColor<DequeueNodePixelCommand>(int, RGBW);

// Template specialization for RingCoordinate
template <>
int LedTableApi::convertToNodeId<RingCoordinate>(const RingCoordinate &coordinate) {
    return clusterManager.getNodeId(coordinate);
}

// Template specialization for Cartesian2dCoordinate
template <>
int LedTableApi::convertToNodeId<Cartesian2dCoordinate>(const Cartesian2dCoordinate &coordinate) {
    return clusterManager.getNodeId(coordinate);
}

// Template specialization for CubeCoordinate
template <>
int LedTableApi::convertToNodeId<CubeCoordinate>(const CubeCoordinate &coordinate) {
    return clusterManager.getNodeId(coordinate);
}

void LedTableApi::fillNode(int nodeId, RGBW color) {
    std::cout << "in root fill with node: " << nodeId << std::endl;
    performClusterOperationReturningVoid<FillNodeCommand>(nodeId, color);
}

void LedTableApi::fillNode(RingCoordinate coordinate, RGBW color) {
    int nodeId = convertToNodeId(coordinate);
    std::cout << "in ring fill with node: " << nodeId << std::endl;
    performClusterOperationReturningVoid<FillNodeCommand>(nodeId, color);
}

void LedTableApi::fillNode(CubeCoordinate coordinate, RGBW color) {
    int nodeId = convertToNodeId(coordinate);
    std::cout << "in cube fill with node: " << nodeId << std::endl;
    performClusterOperationReturningVoid<FillNodeCommand>(nodeId, color);
}

void LedTableApi::fillNode(Cartesian2dCoordinate coordinate, RGBW color) {
    int nodeId = convertToNodeId(coordinate);
    std::cout << "in cart2d fill with node: " << nodeId << std::endl;
    performClusterOperationReturningVoid<FillNodeCommand>(nodeId, color);
}

void LedTableApi::fillNode(int nodeId, const std::vector<RGBW> &colors, RGBW padColor) {
    performClusterOperationReturningVoid<BlitNodeCommand>(nodeId, colors, padColor);
}

void LedTableApi::fillNode(RingCoordinate coordinate, const std::vector<RGBW> &colors, RGBW padColor) {
    int nodeId = convertToNodeId(coordinate);
    std::cout << "api call with ringcoord to fillNode: ";
    std::cout << std::hex << static_cast<RGBW>(padColor) << std::endl;
    performClusterOperationReturningVoid<BlitNodeCommand>(nodeId, colors, padColor);
}

void LedTableApi::fillNode(CubeCoordinate coordinate, const std::vector<RGBW> &colors, RGBW padColor) {
    int nodeId = convertToNodeId(coordinate);
    performClusterOperationReturningVoid<BlitNodeCommand>(nodeId, colors, padColor);
}

void LedTableApi::fillNode(Cartesian2dCoordinate coordinate, const std::vector<RGBW> &colors, RGBW padColor) {
    int nodeId = convertToNodeId(coordinate);
    performClusterOperationReturningVoid<BlitNodeCommand>(nodeId, colors, padColor);
}

// Implementations of the public API methods using the template function
void LedTableApi::setNodePixel(int nodeId, int pixelIndex, RGBW color) {
    performClusterOperationReturningVoid<SetNodePixelCommand>(nodeId, pixelIndex, color);
}

void LedTableApi::setNodePixel(RingCoordinate coordinate, int pixelIndex, RGBW color) {
    int nodeId = convertToNodeId(coordinate);
    performClusterOperationReturningVoid<SetNodePixelCommand>(nodeId, pixelIndex, color);
}

void LedTableApi::setNodePixel(CubeCoordinate coordinate, int pixelIndex, RGBW color) {
    int nodeId = convertToNodeId(coordinate);
    performClusterOperationReturningVoid<SetNodePixelCommand>(nodeId, pixelIndex, color);
}

void LedTableApi::setNodePixel(Cartesian2dCoordinate coordinate, int pixelIndex, RGBW color) {
    int nodeId = convertToNodeId(coordinate);
    performClusterOperationReturningVoid<SetNodePixelCommand>(nodeId, pixelIndex, color);
}

// Implementations of the public API methods using the template function
RGBW LedTableApi::queueNodePixel(int nodeId, RGBW color) {
    return performClusterOperationReturningColor<QueueNodePixelCommand>(nodeId, color);
}

RGBW LedTableApi::queueNodePixel(RingCoordinate coordinate, RGBW color) {
    int nodeId = convertToNodeId(coordinate);
    return performClusterOperationReturningColor<QueueNodePixelCommand>(nodeId, color);
}

RGBW LedTableApi::queueNodePixel(CubeCoordinate coordinate, RGBW color) {
    int nodeId = convertToNodeId(coordinate);
    return performClusterOperationReturningColor<QueueNodePixelCommand>(nodeId, color);
}

RGBW LedTableApi::queueNodePixel(Cartesian2dCoordinate coordinate, RGBW color) {
    int nodeId = convertToNodeId(coordinate);
    return performClusterOperationReturningColor<QueueNodePixelCommand>(nodeId, color);
}

// Implementations of the public API methods using the template function
RGBW LedTableApi::dequeueNodePixel(int nodeId, RGBW color) {
    return performClusterOperationReturningColor<DequeueNodePixelCommand>(nodeId, color);
}

RGBW LedTableApi::dequeueNodePixel(RingCoordinate coordinate, RGBW color) {
    int nodeId = convertToNodeId(coordinate);
    return performClusterOperationReturningColor<DequeueNodePixelCommand>(nodeId, color);
}

RGBW LedTableApi::dequeueNodePixel(CubeCoordinate coordinate, RGBW color) {
    int nodeId = convertToNodeId(coordinate);
    return performClusterOperationReturningColor<DequeueNodePixelCommand>(nodeId, color);
}

RGBW LedTableApi::dequeueNodePixel(Cartesian2dCoordinate coordinate, RGBW color) {
    int nodeId = convertToNodeId(coordinate);
    return performClusterOperationReturningColor<DequeueNodePixelCommand>(nodeId, color);
}

void LedTableApi::setSuppressMessages(bool newValue) {}
void LedTableApi::setSuppressRefresh(bool newValue) {}
