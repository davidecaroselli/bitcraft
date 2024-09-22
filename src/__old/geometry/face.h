//
// Created by Davide Caroselli on 17/09/24.
//

#ifndef BITCRAFT_FACE_H
#define BITCRAFT_FACE_H


#include "vertex.h"
#include "color.h"


struct face_t {
    vertex_t vs[3];
    vertex_t normal;
    color_t color;

    face_t(const vertex_t &v1, const vertex_t &v2, const vertex_t &v3,
           const color_t &c = {1, 1, 1}, const vertex_t &n = {0, 0, 0});


    const vertex_t &operator[](unsigned int i) const {
        return vs[i];
    }

    face_t operator*(const matrix_t &m) const;

    face_t &operator*=(const matrix_t &m);

};


#endif //BITCRAFT_FACE_H
