#include "Cluster.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

Cluster::Cluster(int id, const std::vector<NodeConfig> &configs)
    : id(id) {
    initializeNodes(configs);
    initializePixelBuffer();
}

void Cluster::initializeNodes(const std::vector<NodeConfig> &configs) {
    nodes.clear();
    nodeIdToIndex.clear();

    int currentStartIndex = 0;
    for (const auto &config : configs) {
        // Directly constructs the Node in place within the vector
        nodes.emplace_back(config.nodeId, currentStartIndex, config.pixelCount);
        nodeIdToIndex[config.nodeId] = nodes.size() - 1;
        currentStartIndex += config.pixelCount;
    }
    nodes.shrink_to_fit(); // Optimize memory usage after setting up the nodes
}

void Cluster::initializePixelBuffer() {
    int totalPixels = 0;
    for (const auto &node : nodes) {
        totalPixels += node.getPixelCount();
    }
    pixelBuffer.resize(totalPixels, 0); // Initialize pixelBuffer with default color (0) and fixed size
}

void Cluster::setNodeColor(int nodeId, int32_t color) {
    auto it = nodeIdToIndex.find(nodeId);
    if (it != nodeIdToIndex.end()) {
        Node &node = nodes[it->second];
        int startIndex = node.getStartIndex();
        int ledCount = node.getPixelCount();

        // Use std::fill_n to set the color for the range of LEDs
        std::fill_n(pixelBuffer.begin() + startIndex, ledCount, color);
    }
}

void Cluster::setNodePixel(int nodeId, int ledIndex, int32_t color) {
    auto it = nodeIdToIndex.find(nodeId);
    if (it != nodeIdToIndex.end()) {
        Node &node = nodes[it->second];
        int startIndex = node.getStartIndex();
        int ledCount = node.getPixelCount();

        if (ledIndex < ledCount) {
            pixelBuffer[startIndex + ledIndex] = color; // Corrected indexing
        }
    }
}

int32_t Cluster::queueNodeColor(int nodeId, int32_t color) {
    auto it = nodeIdToIndex.find(nodeId);
    if (it != nodeIdToIndex.end()) {
        Node &node = nodes[it->second];
        int startIndex = node.getStartIndex();
        int ledCount = node.getPixelCount();

        if (ledCount > 0) {
            // Move all pixels up, dropping the last one and inserting the new color at the start
            std::rotate(pixelBuffer.begin() + startIndex,
                        pixelBuffer.begin() + startIndex + ledCount - 1,
                        pixelBuffer.begin() + startIndex + ledCount);
            pixelBuffer[startIndex] = color;
        }
    }
    return 0; // Modify as needed
}

int32_t Cluster::dequeueNodeColor(int nodeId) {
    auto it = nodeIdToIndex.find(nodeId);
    if (it != nodeIdToIndex.end()) {
        Node &node = nodes[it->second];
        int startIndex = node.getStartIndex();
        int ledCount = node.getPixelCount();

        if (ledCount > 0) {
            // Move all pixels down, inserting a default color at the end
            int32_t removedColor = pixelBuffer[startIndex];
            std::rotate(pixelBuffer.begin() + startIndex,
                        pixelBuffer.begin() + startIndex + 1,
                        pixelBuffer.begin() + startIndex + ledCount);
            pixelBuffer[startIndex + ledCount - 1] = 0; // Default color
            return removedColor;
        }
    }
    return 0; // Modify as needed
}

const Node *Cluster::getNode(int nodeId) const {
    auto it = nodeIdToIndex.find(nodeId);
    if (it != nodeIdToIndex.end()) {
        return &nodes[it->second];
    }
    return nullptr; // Node not found
}

int Cluster::getId() const {
    return id;
}

int Cluster::getPixelCount() const {
    return pixelBuffer.size();
}

void Cluster::fillNode(int nodeId, const std::vector<int32_t> &colors, int32_t padColor, bool pad) {
    auto it = nodeIdToIndex.find(nodeId);
    if (it != nodeIdToIndex.end()) {
        Node &node = nodes[it->second];
        int startIndex = node.getStartIndex();
        int ledCount = node.getPixelCount();
        int fillCount = std::min(static_cast<int>(colors.size()), ledCount);

        std::copy(colors.begin(), colors.begin() + fillCount, pixelBuffer.begin() + startIndex);

        if (pad && fillCount < ledCount) {
            std::fill_n(pixelBuffer.begin() + startIndex + fillCount, ledCount - fillCount, padColor);
        }
    }
}

void Cluster::fillNode(int nodeId, int32_t color) {
    auto it = nodeIdToIndex.find(nodeId);
    if (it != nodeIdToIndex.end()) {
        Node &node = nodes[it->second];
        std::fill_n(pixelBuffer.begin() + node.getStartIndex(), node.getPixelCount(), color);
    }
}

void Cluster::fillNode(int nodeId, const std::vector<int32_t> &colors) {
    fillNode(nodeId, colors, 0, false);
}

void Cluster::fillNode(int nodeId, const std::vector<int32_t> &colors, int32_t padColor) {
    fillNode(nodeId, colors, padColor, true);
}

void Cluster::fillBuffer(int32_t color) {
    std::fill(pixelBuffer.begin(), pixelBuffer.end(), color);
}

void Cluster::fillBuffer(const std::vector<int32_t> &colors) {
    int fillCount = static_cast<int>(std::min(colors.size(), pixelBuffer.size()));

    std::copy(colors.begin(), colors.begin() + fillCount, pixelBuffer.begin());

    if (fillCount < pixelBuffer.size()) {
        std::fill(pixelBuffer.begin() + fillCount, pixelBuffer.end(), 0); // Default padding color
    }
}

void Cluster::fillBuffer(const std::vector<int32_t> &colors, int32_t padColor) {
    int fillCount = static_cast<int>(std::min(colors.size(), pixelBuffer.size()));

    std::copy(colors.begin(), colors.begin() + fillCount, pixelBuffer.begin());

    if (fillCount < pixelBuffer.size()) {
        std::fill(pixelBuffer.begin() + fillCount, pixelBuffer.end(), padColor);
    }
}

std::vector<int32_t> Cluster::getPixelBuffer() const  {
    // Returns a copy of the entire pixel buffer, ensuring immutability
    return pixelBuffer;
}

std::vector<int32_t> Cluster::getNodePixelBuffer(int nodeId) const {
    // Returns a copy of the pixel buffer for the specified node, ensuring immutability
    auto it = nodeIdToIndex.find(nodeId);
    if (it != nodeIdToIndex.end()) {
        const Node &node = nodes[it->second];
        int startIndex = node.getStartIndex();
        int ledCount = node.getPixelCount();
        return std::vector<int32_t>(pixelBuffer.begin() + startIndex, pixelBuffer.begin() + startIndex + ledCount);
    }

    // Return an empty vector if the node ID is not found
    return std::vector<int32_t>();
}

// for debugging, output a string representing a node state
std::string Cluster::nodeAscii(int nodeId) const {
    auto it = nodeIdToIndex.find(nodeId);
    if (it != nodeIdToIndex.end()) {
        const Node &node = nodes[it->second];
        int startIndex = node.getStartIndex();
        int ledCount = node.getPixelCount();

        // Ensure ledCount is 8 for this representation to be accurate
        if (ledCount != 8) {
            return "Error: Node does not have 8 LEDs\n";
        }

        std::ostringstream asciiArt;
        // Top row (North, North-East, East)
        asciiArt << " " << (pixelBuffer[startIndex + 0] ? 'x' : 'o') << " ";
        asciiArt << (pixelBuffer[startIndex + 1] ? 'x' : 'o') << " ";
        asciiArt << (pixelBuffer[startIndex + 2] ? 'x' : 'o') << "\n";

        // Middle row (West, Node ID, East)
        asciiArt << (pixelBuffer[startIndex + 7] ? 'x' : 'o') << " ";
        asciiArt << std::setw(3) << nodeId << " ";
        asciiArt << (pixelBuffer[startIndex + 3] ? 'x' : 'o') << "\n";

        // Bottom row (South-West, South, South-East)
        asciiArt << " " << (pixelBuffer[startIndex + 6] ? 'x' : 'o') << " ";
        asciiArt << (pixelBuffer[startIndex + 5] ? 'x' : 'o') << " ";
        asciiArt << (pixelBuffer[startIndex + 4] ? 'x' : 'o') << "\n";

        return asciiArt.str();
    }
    return "Node not found\n";
}

