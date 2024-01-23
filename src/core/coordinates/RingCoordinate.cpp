#include "RingCoordinate.h"

RingCoordinate::RingCoordinate(int ring, int index) : ring_(ring), index_(index) {}

bool RingCoordinate::operator==(const Coordinate& other) const {
    if (const RingCoordinate* otherRing = dynamic_cast<const RingCoordinate*>(&other)) {
        return ring_ == otherRing->ring_ && index_ == otherRing->index_;
    }
    return false;
}

std::size_t RingCoordinate::hash() const {
    return std::hash<int>()(ring_) ^ std::hash<int>()(index_);
}

int RingCoordinate::getRing() const {
    return ring_;
}

int RingCoordinate::getIndex() const {
    return index_;
}
