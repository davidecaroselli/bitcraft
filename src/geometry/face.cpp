//
// Created by Davide Caroselli on 17/09/24.
//

#include "face.h"

vertex_t face_t::normal() const {
    vertex_t line1 = vs[1] - vs[0];
    vertex_t line2 = vs[2] - vs[0];

    vertex_t norm = {
            line1.y * line2.z - line1.z * line2.y,
            line1.z * line2.x - line1.x * line2.z,
            line1.x * line2.y - line1.y * line2.x
    };

    return norm / norm.length();
}

face_t face_t::operator*(const matrix_t &m) const {
    return {vs[0] * m, vs[1] * m, vs[2] * m, color};
}
