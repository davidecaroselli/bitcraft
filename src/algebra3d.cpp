//
// Created by Davide Caroselli on 11/09/24.
//
#include "algebra3d.h"

matrix_t matrix_t::operator*(const matrix_t &o) const {
    return {.v = {
            {
                    v[0][0] * o.v[0][0] + v[0][1] * o.v[1][0] + v[0][2] * o.v[2][0] + v[0][3] * o.v[3][0],
                    v[0][0] * o.v[0][1] + v[0][1] * o.v[1][1] + v[0][2] * o.v[2][1] + v[0][3] * o.v[3][1],
                    v[0][0] * o.v[0][2] + v[0][1] * o.v[1][2] + v[0][2] * o.v[2][2] + v[0][3] * o.v[3][2],
                    v[0][0] * o.v[0][3] + v[0][1] * o.v[1][3] + v[0][2] * o.v[2][3] + v[0][3] * o.v[3][3]
            },
            {
                    v[1][0] * o.v[0][0] + v[1][1] * o.v[1][0] + v[1][2] * o.v[2][0] + v[1][3] * o.v[3][0],
                    v[1][0] * o.v[0][1] + v[1][1] * o.v[1][1] + v[1][2] * o.v[2][1] + v[1][3] * o.v[3][1],
                    v[1][0] * o.v[0][2] + v[1][1] * o.v[1][2] + v[1][2] * o.v[2][2] + v[1][3] * o.v[3][2],
                    v[1][0] * o.v[0][3] + v[1][1] * o.v[1][3] + v[1][2] * o.v[2][3] + v[1][3] * o.v[3][3]
            },
            {
                    v[2][0] * o.v[0][0] + v[2][1] * o.v[1][0] + v[2][2] * o.v[2][0] + v[2][3] * o.v[3][0],
                    v[2][0] * o.v[0][1] + v[2][1] * o.v[1][1] + v[2][2] * o.v[2][1] + v[2][3] * o.v[3][1],
                    v[2][0] * o.v[0][2] + v[2][1] * o.v[1][2] + v[2][2] * o.v[2][2] + v[2][3] * o.v[3][2],
                    v[2][0] * o.v[0][3] + v[2][1] * o.v[1][3] + v[2][2] * o.v[2][3] + v[2][3] * o.v[3][3]
            },
            {
                    v[3][0] * o.v[0][0] + v[3][1] * o.v[1][0] + v[3][2] * o.v[2][0] + v[3][3] * o.v[3][0],
                    v[3][0] * o.v[0][1] + v[3][1] * o.v[1][1] + v[3][2] * o.v[2][1] + v[3][3] * o.v[3][1],
                    v[3][0] * o.v[0][2] + v[3][1] * o.v[1][2] + v[3][2] * o.v[2][2] + v[3][3] * o.v[3][2],
                    v[3][0] * o.v[0][3] + v[3][1] * o.v[1][3] + v[3][2] * o.v[2][3] + v[3][3] * o.v[3][3]
            }
    }};
}

matrix_t matrix_t::Scale(float x, float y, float z) {
    return {{
                    {x, 0, 0, 0},
                    {0, y, 0, 0},
                    {0, 0, z, 0},
                    {0, 0, 0, 1}
            }};
}

matrix_t matrix_t::Translate(float x, float y, float z) {
    return {{
                    {1, 0, 0, 0},
                    {0, 1, 0, 0},
                    {0, 0, 1, 0},
                    {x, y, z, 1}
            }};
}

matrix_t matrix_t::Rotate(float ax, float ay, float az) {
    bool hasRotation = false;
    matrix_t rotation = {{
                                 {1, 0, 0, 0},
                                 {0, 1, 0, 0},
                                 {0, 0, 1, 0},
                                 {0, 0, 0, 1}
                         }};

    if (ax != 0) {
        matrix_t rx = {{
                               {1, 0, 0, 0},
                               {0, cos(ax), -sin(ax), 0},
                               {0, sin(ax), cos(ax), 0},
                               {0, 0, 0, 1}
                       }};

        rotation = rx;
        hasRotation = true;
    }

    if (ay != 0) {
        matrix_t ry = {{
                               {cos(ay), 0, sin(ay), 0},
                               {0, 1, 0, 0},
                               {-sin(ay), 0, cos(ay), 0},
                               {0, 0, 0, 1}
                       }};

        if (!hasRotation) {
            rotation = ry;
            hasRotation = true;
        } else {
            rotation = rotation * ry;
        }
    }

    if (az != 0) {
        matrix_t rz = {{
                               {cos(az), -sin(az), 0, 0},
                               {sin(az), cos(az), 0, 0},
                               {0, 0, 1, 0},
                               {0, 0, 0, 1}
                       }};

        if (!hasRotation)
            rotation = rz;
        else
            rotation = rotation * rz;
    }

    return rotation;
}

mesh_t::mesh_t(const std::vector<triangle_t> &faces) : faces(faces) {}

vertex_t triangle_t::normal() const {
    vertex_t line1 = v[1] - v[0];
    vertex_t line2 = v[2] - v[0];

    vertex_t norm = {
            line1.y * line2.z - line1.z * line2.y,
            line1.z * line2.x - line1.x * line2.z,
            line1.x * line2.y - line1.y * line2.x
    };
    norm.normalize();

    return norm;
}

triangle_t triangle_t::operator*(const matrix_t &m) const {
    return {v[0] * m, v[1] * m, v[2] * m};
}

float vertex_t::length() const {
    return sqrtf(x * x + y * y + z * z);
}

vertex_t vertex_t::operator*(const matrix_t &m) const {
    float ox = x * m.v[0][0] + y * m.v[1][0] + z * m.v[2][0] + m.v[3][0];
    float oy = x * m.v[0][1] + y * m.v[1][1] + z * m.v[2][1] + m.v[3][1];
    float oz = x * m.v[0][2] + y * m.v[1][2] + z * m.v[2][2] + m.v[3][2];
    float ow = x * m.v[0][3] + y * m.v[1][3] + z * m.v[2][3] + m.v[3][3];

    if (ow == 0) ow = 1;

    return {ox / ow, oy / ow, oz / ow};
}

float vertex_t::operator*(const vertex_t &v) const {
    return x * v.x + y * v.y + z * v.z;
}

vertex_t vertex_t::operator-(const vertex_t &v) const {
    return {x - v.x, y - v.y, z - v.z};
}

void vertex_t::normalize() {
    float l = length();
    x /= l;
    y /= l;
    z /= l;
}
