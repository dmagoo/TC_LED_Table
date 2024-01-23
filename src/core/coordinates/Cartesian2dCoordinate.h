#ifndef CARTESIAN2DCOORDINATE_H
#define CARTESIAN2DCOORDINATE_H

#include "Coordinate.h"

class Cartesian2dCoordinate : public Coordinate {
public:
    Cartesian2dCoordinate() = default;  // Default constructor to satisfy unordered_map
    Cartesian2dCoordinate(float x, float y);

    bool operator==(const Coordinate& other) const override;

    std::size_t hash() const override;

    float getX() const;
    float getY() const;

private:
    float x_;
    float y_;
};

// Specialize std::hash for Cartesian2dCoordinate in the std namespace
namespace std {
    template<>
    struct hash<Cartesian2dCoordinate> {
        size_t operator()(const Cartesian2dCoordinate& coord) const noexcept {
            // Use Cartesian2dCoordinateCoordinate's hash method here
            return coord.hash();
        }
    };
}


#endif // CARTESIAN2dCOORDINATE_H
