#ifndef CLUSTER_H
#define CLUSTER_H

#include <vector>
#include <unordered_map>
#include <cstdint>
#include <array>
#include <string>

#include "LedTableTypes.h"
#include "Node.h"
#include "NodeConfig.h"

/**
 * Represents a cluster of nodes, each containing a segment of a pixel buffer.
 */
class Cluster {
    int id;
    std::vector<Node> nodes; // Stores all nodes within the cluster
    std::vector<RGBW> pixelBuffer; // Buffer for pixel colors
    std::unordered_map<int, size_t> nodeIdToIndex; // Fast lookup for nodes by their ID
private:
    void initializePixelBuffer();
    void fillNode(int nodeId, const std::vector<RGBW>& colors, RGBW padColor, bool pad);
public:
    /**
     * Constructor for the Cluster.
     * 
     * @param id The ID of the cluster.
     * @param configs The initial configuration for the nodes.
     */
    Cluster(int id, const std::vector<NodeConfig>& configs);

    /**
     * Initializes or updates the nodes in the cluster.
     * 
     * @param nodeConfigs The configuration for the nodes.
     */
    void initializeNodes(const std::vector<NodeConfig>& configs);
  
    /**
     * Sets the color of a specific pixel in a node.
     * 
     * @param nodeId The ID of the node.
     * @param pixelIndex The index of the pixel within the node.
     * @param color The color to set.
     */
    void setNodePixel(int nodeId, int pixelIndex, RGBW color);

    /**
     * Queues a new color at the start of a node's pixel range, pushing other colors forward.
     * The color at the end is removed and returned.
     * 
     * @param nodeId The ID of the node.
     * @param color The new color to queue.
     * @return The color that was removed from the end of the queue.
     */
    RGBW queueNodeColor(int nodeId, RGBW color);

    /**
     * Removes the first color in a node's pixel range and pushes other colors back.
     * The new color is added at the end of the range.
     * 
     * @param nodeId The ID of the node.
     * @param color The new color to queue.
     * @return The color that was removed from the start of the queue.
     */
    RGBW dequeueNodeColor(int nodeId, RGBW color);

    /**
     * Retrieves a pointer to a Node object by its ID.
     * 
     * @param nodeId The ID of the node to retrieve.
     * @return A pointer to the Node object, or nullptr if not found.
     * 
     * @note This method could throw an exception or handle the error if the node is not found.
     */
    const Node* getNode(int nodeId) const;

    int getId() const;

    int getPixelCount() const;

    /**
     * Fills a node with a single color.
     * 
     * @param nodeId The ID of the node.
     * @param color The color to fill the node with.
     */
    void fillNode(int nodeId, RGBW color);

    /**
     * Fills a node with a list of colors and pads extra pixels with padColor.
     * 
     * @param nodeId The ID of the node.
     * @param colors The list of colors to fill the node with.
     * @param padColor The color to pad the extra pixels with.
     */
    void fillNode(int nodeId, const std::vector<RGBW>& colors, RGBW padColor);

    /**
     * Fills a node with a list of colors and pads extra pixels with default (black)
     * 
     * @param nodeId The ID of the node.
     * @param colors The list of colors to fill the node with.
     */
    void fillNode(int nodeId, const std::vector<RGBW>& colors);

    /**
     * Fills the entire buffer with a single color.
     * 
     * @param color The color to fill the buffer with.
     */
    void fillBuffer(RGBW color);

    /**
     * Fills the entire buffer with a list of colors.
     * 
     * @param colors The list of colors to fill the buffer with.
     */
    void fillBuffer(const std::vector<RGBW>& colors);

    /**
     * Fills the buffer with a list of colors and pads extra pixels with padColor.
     * 
     * @param colors The list of colors to fill the buffer with.
     * @param padColor The color to pad the extra pixels with.
     */
    void fillBuffer(const std::vector<RGBW>& colors, RGBW padColor);

    /**
     * Returns an immutable buffer of the entire cluster
     */
    std::vector<RGBW> getPixelBuffer() const;

    /**
     * Returns an immutable buffer of a single node
     * @param nodeId The ID of the node.
     */
    std::vector<RGBW> getNodePixelBuffer(int nodeId) const;

    /**
     * Returns a string representation of a single node
     */
    std::string nodeAscii(int nodeId) const;
};

#endif // CLUSTER_H
