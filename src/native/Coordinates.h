#ifndef COORDINATES_H
#define COORDINATES_H

struct RingCoordinate {
    int ring;
    int index;
};

struct CubeCoordinate {
    int q, r, s;
};

struct CartesianCoordinate {
    float x, y;
};

// Other related functions...

#endif // COORDINATES_H
