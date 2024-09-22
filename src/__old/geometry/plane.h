//
// Created by Davide Caroselli on 21/09/24.
//

#ifndef BITCRAFT_PLANE_H
#define BITCRAFT_PLANE_H

#include "vertex.h"

struct plane_t {
    plane_t(const vertex_t &p, const vertex_t &n) : point(p), normal(n / n.length()) {};

    [[nodiscard]] vertex_t Intersection(const vertex_t &lineStart, const vertex_t &lineEnd) const;

    [[nodiscard]] float Distance(const vertex_t &p) const;

private:
    vertex_t point, normal;
};


#endif //BITCRAFT_PLANE_H
