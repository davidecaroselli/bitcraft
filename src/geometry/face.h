//
// Created by Davide Caroselli on 17/09/24.
//

#ifndef BITCRAFT_FACE_H
#define BITCRAFT_FACE_H


#include "vertex.h"
#include "color.h"


struct face_t {
    vertex_t vs[3];
    color_t color{1, 1, 1};

    face_t(const vertex_t &v1, const vertex_t &v2, const vertex_t &v3, const color_t &c = {1, 1, 1})
            : vs{v1, v2, v3}, color(c) {}

    const vertex_t &operator[](unsigned int i) const {
        return vs[i];
    }

    [[nodiscard]] vertex_t normal() const;

    face_t operator*(const matrix_t &m) const;
};


#endif //BITCRAFT_FACE_H
