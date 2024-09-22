//
// Created by Davide Caroselli on 17/09/24.
//

#include "face.h"

face_t::face_t(const vertex_t &v1, const vertex_t &v2, const vertex_t &v3, const color_t &c, const vertex_t &n)
        : vs{v1, v2, v3}, color(c), normal(n) {
    if (normal.length() == 0) {
        vertex_t line1 = vs[1] - vs[0];
        vertex_t line2 = vs[2] - vs[0];

        normal = {
                line1.y * line2.z - line1.z * line2.y,
                line1.z * line2.x - line1.x * line2.z,
                line1.x * line2.y - line1.y * line2.x
        };
    }

    normal /= normal.length();
}

face_t face_t::operator*(const matrix_t &m) const {
    return {vs[0] * m, vs[1] * m, vs[2] * m, color};
}

face_t &face_t::operator*=(const matrix_t &m) {
    vs[0] *= m;
    vs[1] *= m;
    vs[2] *= m;

    return *this;
}
