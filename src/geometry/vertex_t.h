//
// Created by Davide Caroselli on 17/09/24.
//

#ifndef BITCRAFT_VERTEX_T_H
#define BITCRAFT_VERTEX_T_H

#include "matrix_t.h"
#include <cmath>

struct vertex_t {
    float x, y, z;

    vertex_t() = default;

    vertex_t(float x, float y, float z) : x(x), y(y), z(z) {}

    [[nodiscard]] inline vertex_t cross(const vertex_t &v) const {
        return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
    }

    [[nodiscard]] inline float length() const {
        return sqrtf(x * x + y * y + z * z);
    }

    inline vertex_t operator-(const vertex_t &v) const {
        return {x - v.x, y - v.y, z - v.z};
    }

    inline vertex_t operator+(const vertex_t &v) const {
        return {x + v.x, y + v.y, z + v.z};
    }

    inline vertex_t operator/(float v) const {
        return {x / v, y / v, z / v};
    }

    // dot product
    inline float operator*(const vertex_t &v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    inline vertex_t operator*(const matrix_t &m) const {
        float ox = x * m(0, 0) + y * m(1, 0) + z * m(2, 0) + m(3, 0);
        float oy = x * m(0, 1) + y * m(1, 1) + z * m(2, 1) + m(3, 1);
        float oz = x * m(0, 2) + y * m(1, 2) + z * m(2, 2) + m(3, 2);
        float ow = x * m(0, 3) + y * m(1, 3) + z * m(2, 3) + m(3, 3);

        if (ow == 0) ow = 1;

        return {ox / ow, oy / ow, oz / ow};
    }
};


#endif //BITCRAFT_VERTEX_T_H
