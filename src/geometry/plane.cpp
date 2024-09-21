//
// Created by Davide Caroselli on 21/09/24.
//

#include "plane.h"

vertex_t plane_t::Intersection(const vertex_t &lineStart, const vertex_t &lineEnd) const  {
    float d = normal * point;
    float ad = lineStart * normal;
    float bd = lineEnd * normal;
    float t = (d - ad) / (bd - ad);

    return lineStart + (lineEnd - lineStart) * t;
}

float plane_t::Distance(const vertex_t &p) const {
    return (normal * p) - (normal * point);
}
