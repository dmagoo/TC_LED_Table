#ifndef NODECONFIG_H
#define NODECONFIG_H
#include <array>
#include <vector>

struct NodeConfig {
    int nodeId;
    int pixelCount;
    std::array<int, 3> cubeCoordinates;
    std::array<int, 2> ringCoordinates;
    std::array<float, 2> cartesian2dCoordinates;

public:
    NodeConfig(int id, int count, const std::array<int, 2> ringCoords,
                           const std::array<int, 3> cubeCoords, const std::array<float, 2> cartesianCoords)
        : nodeId(id), pixelCount(count), ringCoordinates(ringCoords),
          cubeCoordinates(cubeCoords), cartesian2dCoordinates(cartesianCoords)
    {
    }

    int getId() const;
    int getPixelCount() const;
    const std::array<int, 2> getRingCoordinates() const;
    const std::array<int, 3> getCubeCoordinates() const;
    const std::array<float, 2> getCartesian2dCoordinates() const;
};

#endif // NODECONFIG_H