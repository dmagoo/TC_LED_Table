#include "Cartesian2dCoordinate.h"

Cartesian2dCoordinate::Cartesian2dCoordinate(float x, float y) : x_(x), y_(y) {}

bool Cartesian2dCoordinate::operator==(const Coordinate& other) const {
    if (const Cartesian2dCoordinate* otherCartesian2d = dynamic_cast<const Cartesian2dCoordinate*>(&other)) {
        return x_ == otherCartesian2d->x_ && y_ == otherCartesian2d->y_;
    }
    return false;
}

std::size_t Cartesian2dCoordinate::hash() const {
    return std::hash<float>()(x_) ^ std::hash<float>()(y_);
}

float Cartesian2dCoordinate::getX() const {
    return x_;
}

float Cartesian2dCoordinate::getY() const {
    return y_;
}
