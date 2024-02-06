#include "CubeCoordinate.h"

CubeCoordinate::CubeCoordinate(int q, int r, int s) : q_(q), r_(r), s_(s) {}

bool CubeCoordinate::operator==(const Coordinate &other) const {
    if (const CubeCoordinate *otherCube = dynamic_cast<const CubeCoordinate *>(&other)) {
        return q_ == otherCube->q_ && r_ == otherCube->r_ && s_ == otherCube->s_;
    }
    return false;
}

std::size_t CubeCoordinate::hash() const {
    return std::hash<int>()(q_) ^ std::hash<int>()(r_) ^ std::hash<int>()(s_);
}

int CubeCoordinate::getQ() const {
    return q_;
}

int CubeCoordinate::getR() const {
    return r_;
}

int CubeCoordinate::getS() const {
    return s_;
}
