#include "Node.h"

Node::Node(int id, int startIdx, int count) 
    : id(id), startIndex(startIdx), pixelCount(count) {
}

int Node::getStartIndex() const {
    return startIndex;
}

int Node::getPixelCount() const {
    return pixelCount;
}

int Node::getId() const {
    return id; // Return the value of the internal id member variable
}
