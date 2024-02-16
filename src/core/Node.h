#pragma once
#include <cstdint>
#include <vector>

#include "sensors/TouchSensor.h"

/**
 * Represents a node within a cluster, containing a segment of the pixel buffer.
 */
class Node {
private:
    int id;         // Unique identifier for the node
    int startIndex; // Starting index in the pixel buffer
    int pixelCount; // Number of pixels in this node

public:
    TouchSensor touchSensor;

    /**
     * Constructor for the Node.
     *
     * @param id The ID of the node.
     * @param startIdx The starting index of this node in the cluster's pixel buffer.
     * @param pixelCount The number of pixels in this node.
     */
    Node(int id, int startIdx, int pixelCount);

    // Getter methods
    int getStartIndex() const;
    int getPixelCount() const;
    int getId() const;
};
