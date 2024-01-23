#ifndef RINGCOORDINATE_H
#define RINGCOORDINATE_H

#include "Coordinate.h"

class RingCoordinate : public Coordinate {
public:
    RingCoordinate() = default;  // Default constructor to satisfy unordered_map
    RingCoordinate(int ring, int index);

    bool operator==(const Coordinate& other) const override;

    std::size_t hash() const override;

    int getRing() const;
    int getIndex() const;

private:
    int ring_;
    int index_;
};

// Specialize std::hash for RingCoordinate in the std namespace
namespace std {
    template<>
    struct hash<RingCoordinate> {
        size_t operator()(const RingCoordinate& coord) const noexcept {
            // Use RingCoordinate's hash method here
            return coord.hash();
        }
    };
}

#endif // RINGCOORDINATE_H
