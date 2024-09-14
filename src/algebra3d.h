//
// Created by Davide Caroselli on 11/09/24.
//

#ifndef ENGINE3D_ALGEBRA3D_H
#define ENGINE3D_ALGEBRA3D_H

#include <vector>

struct matrix_t {
    float v[4][4];

    static matrix_t Scale(float v) {
        return Scale(v, v, v);
    }

    static matrix_t Scale(float x, float y, float z);

    static matrix_t Translate(float x, float y, float z);

    static matrix_t Rotate(float ax, float ay, float az);

    matrix_t operator*(const matrix_t &o) const;
};

struct vertex_t {
    float x, y, z;

    vertex_t operator*(const matrix_t &m) const;
};

struct triangle_t {
    vertex_t v[3];

    triangle_t operator*(const matrix_t &m) const;
};

struct mesh_t {
    std::vector<triangle_t> faces;

    explicit mesh_t(const std::vector<triangle_t> &faces);
};

#endif //ENGINE3D_ALGEBRA3D_H
