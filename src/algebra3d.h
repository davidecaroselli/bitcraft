//
// Created by Davide Caroselli on 11/09/24.
//

#ifndef ENGINE3D_ALGEBRA3D_H
#define ENGINE3D_ALGEBRA3D_H

#include <vector>

struct matrix_t {
    float v[4][4];

    matrix_t operator*(const matrix_t &o) const;
};

struct vertex_t {
    float x, y, z;
};

struct triangle_t {
    vertex_t v[3];


};

struct mesh_t {
    std::vector<triangle_t> faces;

    explicit mesh_t(const std::vector<triangle_t> &faces);

    void transform(const matrix_t &m);
};

#endif //ENGINE3D_ALGEBRA3D_H
