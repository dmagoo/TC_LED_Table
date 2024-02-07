#include "LedTableApi.h"
#include "core/ClusterMessage.h"
#include "core/node_geometry.h"
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
WRGB LedTableApi::performClusterOperationReturningColor(int nodeId, Args... args) {
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
template void LedTableApi::performClusterOperationReturningVoid<FillNodeCommand, WRGB>(int, WRGB);
template WRGB LedTableApi::performClusterOperationReturningColor<QueueNodePixelCommand, WRGB>(int, WRGB);
template WRGB LedTableApi::performClusterOperationReturningColor<DequeueNodePixelCommand>(int, WRGB);

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

void LedTableApi::fillNode(int nodeId, WRGB color) {
    //////std::couttt << "in root fill with node: " << nodeId << std::endl;
    performClusterOperationReturningVoid<FillNodeCommand>(nodeId, color);
}

void LedTableApi::fillNode(RingCoordinate coordinate, WRGB color) {
    int nodeId = convertToNodeId(coordinate);
    performClusterOperationReturningVoid<FillNodeCommand>(nodeId, color);
}

void LedTableApi::fillNode(CubeCoordinate coordinate, WRGB color) {
    int nodeId = convertToNodeId(coordinate);
    //////std::couttt << "in cube fill with node: " << nodeId << std::endl;
    performClusterOperationReturningVoid<FillNodeCommand>(nodeId, color);
}

void LedTableApi::fillNode(Cartesian2dCoordinate coordinate, WRGB color) {
    int nodeId = convertToNodeId(coordinate);
    ////std::coutt << "in cart2d fill with node: " << nodeId << std::endl;
    performClusterOperationReturningVoid<FillNodeCommand>(nodeId, color);
}

void LedTableApi::fillNode(int nodeId, const std::vector<WRGB> &colors, WRGB padColor) {
    performClusterOperationReturningVoid<BlitNodeCommand>(nodeId, colors, padColor);
}

void LedTableApi::fillNode(RingCoordinate coordinate, const std::vector<WRGB> &colors, WRGB padColor) {
    int nodeId = convertToNodeId(coordinate);
    ////std::couttt << "api call with ringcoord to fillNode: ";
    ////std::couttt << std::hex << static_cast<WRGB>(padColor) << std::endl;
    performClusterOperationReturningVoid<BlitNodeCommand>(nodeId, colors, padColor);
}

void LedTableApi::fillNode(CubeCoordinate coordinate, const std::vector<WRGB> &colors, WRGB padColor) {
    int nodeId = convertToNodeId(coordinate);
    performClusterOperationReturningVoid<BlitNodeCommand>(nodeId, colors, padColor);
}

void LedTableApi::fillNode(Cartesian2dCoordinate coordinate, const std::vector<WRGB> &colors, WRGB padColor) {
    int nodeId = convertToNodeId(coordinate);
    performClusterOperationReturningVoid<BlitNodeCommand>(nodeId, colors, padColor);
}

// Implementations of the public API methods using the template function
void LedTableApi::setNodePixel(int nodeId, int pixelIndex, WRGB color) {
    performClusterOperationReturningVoid<SetNodePixelCommand>(nodeId, pixelIndex, color);
}

void LedTableApi::setNodePixel(RingCoordinate coordinate, int pixelIndex, WRGB color) {
    int nodeId = convertToNodeId(coordinate);
    performClusterOperationReturningVoid<SetNodePixelCommand>(nodeId, pixelIndex, color);
}

void LedTableApi::setNodePixel(CubeCoordinate coordinate, int pixelIndex, WRGB color) {
    int nodeId = convertToNodeId(coordinate);
    performClusterOperationReturningVoid<SetNodePixelCommand>(nodeId, pixelIndex, color);
}

void LedTableApi::setNodePixel(Cartesian2dCoordinate coordinate, int pixelIndex, WRGB color) {
    int nodeId = convertToNodeId(coordinate);
    performClusterOperationReturningVoid<SetNodePixelCommand>(nodeId, pixelIndex, color);
}

// Implementations of the public API methods using the template function
WRGB LedTableApi::queueNodePixel(int nodeId, WRGB color) {
    return performClusterOperationReturningColor<QueueNodePixelCommand>(nodeId, color);
}

WRGB LedTableApi::queueNodePixel(RingCoordinate coordinate, WRGB color) {
    int nodeId = convertToNodeId(coordinate);
    return performClusterOperationReturningColor<QueueNodePixelCommand>(nodeId, color);
}

WRGB LedTableApi::queueNodePixel(CubeCoordinate coordinate, WRGB color) {
    int nodeId = convertToNodeId(coordinate);
    return performClusterOperationReturningColor<QueueNodePixelCommand>(nodeId, color);
}

WRGB LedTableApi::queueNodePixel(Cartesian2dCoordinate coordinate, WRGB color) {
    int nodeId = convertToNodeId(coordinate);
    return performClusterOperationReturningColor<QueueNodePixelCommand>(nodeId, color);
}

// Implementations of the public API methods using the template function
WRGB LedTableApi::dequeueNodePixel(int nodeId, WRGB color) {
    return performClusterOperationReturningColor<DequeueNodePixelCommand>(nodeId, color);
}

WRGB LedTableApi::dequeueNodePixel(RingCoordinate coordinate, WRGB color) {
    int nodeId = convertToNodeId(coordinate);
    return performClusterOperationReturningColor<DequeueNodePixelCommand>(nodeId, color);
}

WRGB LedTableApi::dequeueNodePixel(CubeCoordinate coordinate, WRGB color) {
    int nodeId = convertToNodeId(coordinate);
    return performClusterOperationReturningColor<DequeueNodePixelCommand>(nodeId, color);
}

WRGB LedTableApi::dequeueNodePixel(Cartesian2dCoordinate coordinate, WRGB color) {
    int nodeId = convertToNodeId(coordinate);
    return performClusterOperationReturningColor<DequeueNodePixelCommand>(nodeId, color);
}

std::vector<int> LedTableApi::listNodeIds() {
    return clusterManager.listNodeIds();
}

int LedTableApi::getNodeId(const RingCoordinate &coords) const {
    return clusterManager.getNodeId(coords);
}
int LedTableApi::getNodeId(const CubeCoordinate &coords) const {
    return clusterManager.getNodeId(coords);
}
int LedTableApi::getNodeId(const Cartesian2dCoordinate &coords) const {
    return clusterManager.getNodeId(coords);
}

RingCoordinate LedTableApi::getRingCoordinate(int nodeId) const {
    return clusterManager.getRingCoordinate(nodeId);
}
CubeCoordinate LedTableApi::getCubeCoordinate(int nodeId) const {
    return clusterManager.getCubeCoordinate(nodeId);
}
Cartesian2dCoordinate LedTableApi::getCartesian2dCoordinate(int nodeId) const {
    return clusterManager.getCartesian2dCoordinate(nodeId);
}

std::vector<WRGB> LedTableApi::getNodePixelBuffer(int nodeId) {
    int clusterId = clusterManager.getClusterIdFromNodeId(nodeId);
    const Cluster *clusterPtr = clusterManager.getClusterById(clusterId);
    return clusterPtr->getNodePixelBuffer(nodeId);
}
std::vector<WRGB> LedTableApi::getNodePixelBuffer(RingCoordinate coordinate) {
    int nodeId = convertToNodeId(coordinate);
    return getNodePixelBuffer(nodeId);
}
std::vector<WRGB> LedTableApi::getNodePixelBuffer(Cartesian2dCoordinate coordinate) {
    int nodeId = convertToNodeId(coordinate);
    return getNodePixelBuffer(nodeId);
}
std::vector<WRGB> LedTableApi::getNodePixelBuffer(CubeCoordinate coordinate) {
    int nodeId = convertToNodeId(coordinate);
    return getNodePixelBuffer(nodeId);
}

std::vector<int> LedTableApi::getNodePath(int nodeIdA, int nodeIdB) { 
    std::vector<int> values = node_geometry::get_node_path(nodeIdA, nodeIdB, clusterManager);
    return values;
}
std::vector<int> LedTableApi::getNodePath(RingCoordinate coordinateA, RingCoordinate coordinateB) {
    int nodeIdA = convertToNodeId(coordinateA);
    int nodeIdB = convertToNodeId(coordinateB);
    return {};
}
std::vector<int> LedTableApi::getNodePath(Cartesian2dCoordinate coordinateA, Cartesian2dCoordinate coordinateB) {
    int nodeIdA = convertToNodeId(coordinateA);
    int nodeIdB = convertToNodeId(coordinateB);
    return {};
}
std::vector<int> LedTableApi::getNodePath(CubeCoordinate coordinateA, CubeCoordinate coordinateB) {
    int nodeIdA = convertToNodeId(coordinateA);
    int nodeIdB = convertToNodeId(coordinateB);
    return {};
}

std::vector<int> LedTableApi::getNodeNeighbors(int nodeId) {
    std::vector<int> values = node_geometry::get_node_neighbors(nodeId, clusterManager);
    return values;
}
std::vector<int> LedTableApi::getNodeNeighbors(RingCoordinate coordinate) {
    int nodeId = convertToNodeId(coordinate);
    return getNodeNeighbors(nodeId);
}
std::vector<int> LedTableApi::getNodeNeighbors(Cartesian2dCoordinate coordinate) {
    int nodeId = convertToNodeId(coordinate);
    return getNodeNeighbors(nodeId);
}
std::vector<int> LedTableApi::getNodeNeighbors(CubeCoordinate coordinate) {
    int nodeId = convertToNodeId(coordinate);
    return getNodeNeighbors(nodeId);
}

std::tuple<int, int> LedTableApi::getFacingPixelIndexes(int nodeIdA, int nodeIdB) { return {}; }
std::tuple<int, int> LedTableApi::getFacingPixelIndexes(RingCoordinate coordinateA, RingCoordinate coordinateB) {
    int nodeIdA = convertToNodeId(coordinateA);
    int nodeIdB = convertToNodeId(coordinateB);
    return {};
}
std::tuple<int, int> LedTableApi::getFacingPixelIndexes(Cartesian2dCoordinate coordinateA, Cartesian2dCoordinate coordinateB) {
    int nodeIdA = convertToNodeId(coordinateA);
    int nodeIdB = convertToNodeId(coordinateB);
    return {};
}
std::tuple<int, int> LedTableApi::getFacingPixelIndexes(CubeCoordinate coordinateA, CubeCoordinate coordinateB) {
    int nodeIdA = convertToNodeId(coordinateA);
    int nodeIdB = convertToNodeId(coordinateB);
    return {};
}

void LedTableApi::setSuppressMessages(bool newValue) {
    suppressMessages = newValue;
}

// ignores suppressMessges, as this is an explicit command to send a message
// but cannot operate if a clustermessagemanager is not present
void LedTableApi::refresh() {
    clusterManager.forEachCluster([this](Cluster &cluster) { // Capture `this` to access class members
        std::vector<WRGB> buffer = cluster.getPixelBuffer();
        if (this->clusterMessageManager != nullptr) { // Use `this->` to access class members
            BlitBufferCommand command(buffer, 0x00000000);
            this->clusterMessageManager->sendClusterCommand(cluster.getId(), command);
        }
    });
}

void LedTableApi::reset() {
    clusterManager.forEachCluster([this](Cluster &cluster) {
        FillBufferCommand command(0x00000000);
        command.execute(cluster);
        if (!suppressMessages) {
            if (this->clusterMessageManager != nullptr) { // Use `this->` to access class members

                this->clusterMessageManager->sendClusterCommand(cluster.getId(), command);
            }
        }
    });
}
