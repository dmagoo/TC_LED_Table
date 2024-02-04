#include "LedTableApi.h"
#include "core/ClusterMessage.h"
#include <iostream>
// #include <vector>

template <typename CommandType, typename... Args>
void LedTableApi::performClusterOperationReturningVoid(int nodeId, Args... args) {
    int clusterId = clusterManager.getClusterIdFromNodeId(nodeId);
    const Cluster *clusterPtr = clusterManager.getClusterById(clusterId);
std::cout << "LOLOLOL" << std::endl;
    if (clusterPtr) {
std::cout << "got cp" << std::endl;
        Cluster &cluster = *const_cast<Cluster *>(clusterPtr); // Cast to non-const reference
std::cout << "got cluster, i hope" << std::endl;
        if constexpr (sizeof...(args) >= 2) {
std::cout << "got arg2?" << std::endl;
            auto secondArgument = std::get<1>(std::make_tuple(args...));
std::cout << "got it!" << std::endl;
        }
        //// HERE the seond argument is fine!!  Is the syntax below correct?
        CommandType command(nodeId, args...);
std::cout << "made a command" << std::endl;
        // run the command against the local model
        if (!suppressMessages && clusterMessageManager != nullptr) {
std::cout << "about to send a cc" << std::endl;
            clusterMessageManager->sendClusterCommand(clusterId, command);
std::cout << "sent a cc" << std::endl;
        }
std::cout << "going to execute" << std::endl;
        command.execute(cluster);
std::cout << "we executed" << std::endl;
    } else {
std::cout << "we done baddd" << std::endl;
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
    std::cout << "converting ring coord to node id" << std::endl;
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
    try {
        std::cout << "incoming" << std::endl;
        int nodeId = convertToNodeId(coordinate);
        std::cout << "in ring fill with node: " << nodeId << std::endl;
        performClusterOperationReturningVoid<FillNodeCommand>(nodeId, color);
    } catch (const std::exception& e) {
        // Catch standard exceptions
        std::cout << "An exception occurred in fillNode: " << e.what() << std::endl;
    } catch (...) {
        // Catch any other types of exceptions
        std::cout << "An unknown exception occurred in fillNode." << std::endl;
    }
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
