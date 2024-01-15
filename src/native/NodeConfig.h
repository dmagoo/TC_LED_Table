#ifndef NODECONFIG_H
#define NODECONFIG_H

#include "coordinates/RingCoordinate.h"
#include "coordinates/CubeCoordinate.h"
#include "coordinates/Cartesian2dCoordinate.h"

struct NodeConfig {
    int nodeId;
    int pixelCount;
    RingCoordinate ringCoordinate;
    CubeCoordinate cubeCoordinate;
    Cartesian2dCoordinate cartesian2dCoordinate;

public:
    NodeConfig(int id, int count, const RingCoordinate& ringCoord,
               const CubeCoordinate& cubeCoord, const Cartesian2dCoordinate& cartesianCoord)
        : nodeId(id), pixelCount(count), ringCoordinate(ringCoord),
          cubeCoordinate(cubeCoord), cartesian2dCoordinate(cartesianCoord)
    {
    }

    int getId() const;
    int getPixelCount() const;
    RingCoordinate getRingCoordinates() const;
    CubeCoordinate getCubeCoordinates() const;
    Cartesian2dCoordinate getCartesian2dCoordinates() const;
};

#endif // NODECONFIG_H
