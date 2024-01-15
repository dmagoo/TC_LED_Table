#ifndef COORDINATE_H
#define COORDINATE_H

#include <functional>

class Coordinate {
public:
    virtual ~Coordinate() = default;

    virtual bool operator==(const Coordinate& other) const = 0;

    // Hash method for Coordinate objects
    virtual std::size_t hash() const = 0;
};

// Hash function specialization for Coordinate objects
namespace std {
    template <>
    struct hash<Coordinate> {
        std::size_t operator()(const Coordinate& coord) const {
            return coord.hash();
        }
    };
}

#endif // COORDINATE_H
