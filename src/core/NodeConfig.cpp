#include "NodeConfig.h"

int NodeConfig::getId() const { 
    return nodeId; 
}

int NodeConfig::getPixelCount() const { 
    return pixelCount; 
}

RingCoordinate NodeConfig::getRingCoordinates() const { 
    return ringCoordinate; 
}

CubeCoordinate NodeConfig::getCubeCoordinates() const { 
    return cubeCoordinate; 
}

Cartesian2dCoordinate NodeConfig::getCartesian2dCoordinates() const { 
    return cartesian2dCoordinate; 
}
