#ifndef CUBECOORDINATE_H
#define CUBECOORDINATE_H

#include "Coordinate.h"

class CubeCoordinate : public Coordinate {
public:
    CubeCoordinate() = default;  // Default constructor to satisfy unordered_map
    CubeCoordinate(int q, int r, int s);

    bool operator==(const Coordinate& other) const override;

    std::size_t hash() const override;

    int getQ() const;
    int getR() const;
    int getS() const;

private:
    int q_;
    int r_;
    int s_;    
};

// Specialize std::hash for CubeCoordinate in the std namespace
namespace std {
    template<>
    struct hash<CubeCoordinate> {
        size_t operator()(const CubeCoordinate& coord) const noexcept {
            // Use CubeCoordinate's hash method here
            return coord.hash();
        }
    };
}

#endif // CUBECOORDINATE_H
