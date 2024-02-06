#ifndef CUBECOORDINATE_H
#define CUBECOORDINATE_H

#include "Coordinate.h"

class CubeCoordinate : public Coordinate {
public:
    CubeCoordinate() = default; // Default constructor to satisfy unordered_map
    CubeCoordinate(int q, int r, int s);

    bool operator==(const Coordinate &other) const override;

    std::size_t hash() const override;

    int getQ() const;
    int getR() const;
    int getS() const;

    CubeCoordinate CubeCoordinate::operator+(const CubeCoordinate &other) const {
        return CubeCoordinate(this->q_ + other.q_, this->r_ + other.r_, this->s_ + other.s_);
    }

    CubeCoordinate CubeCoordinate::operator-(const CubeCoordinate &other) const {
        return CubeCoordinate(this->q_ - other.q_, this->r_ - other.r_, this->s_ - other.s_);
    }

    // adjacent directions
    static CubeCoordinate North() { return CubeCoordinate(-1, +1, 0); }
    static CubeCoordinate NorthEast() { return CubeCoordinate(0, +1, -1); }
    static CubeCoordinate SouthEast() { return CubeCoordinate(1, 0, -1); }
    static CubeCoordinate South() { return CubeCoordinate(+1, -1, 0); }
    static CubeCoordinate SouthWest() { return CubeCoordinate(0, -1, +1); }
    static CubeCoordinate NorthWest() { return CubeCoordinate(-1, 0, 1); }

private:
    int q_;
    int r_;
    int s_;
};

// Specialize std::hash for CubeCoordinate in the std namespace
namespace std {
template <>
struct hash<CubeCoordinate> {
    size_t operator()(const CubeCoordinate &coord) const noexcept {
        // Use CubeCoordinate's hash method here
        return coord.hash();
    }
};
} // namespace std

#endif // CUBECOORDINATE_H
