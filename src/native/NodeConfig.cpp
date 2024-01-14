#include "NodeConfig.h"

int NodeConfig::getId() const { return nodeId; }
int NodeConfig::getPixelCount() const { return pixelCount; }
const std::array<int, 2> NodeConfig::getRingCoordinates() const { return ringCoordinates; }
const std::array<int, 3> NodeConfig::getCubeCoordinates() const { return cubeCoordinates; }
const std::array<float, 2> NodeConfig::getCartesian2dCoordinates() const { return cartesian2dCoordinates; }
