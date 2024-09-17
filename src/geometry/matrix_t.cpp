//
// Created by Davide Caroselli on 17/09/24.
//

#include <cmath>
#include "matrix_t.h"

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
                               {0, cosf(ax), -sinf(ax), 0},
                               {0, sinf(ax), cosf(ax), 0},
                               {0, 0, 0, 1}
                       }};

        rotation = rx;
        hasRotation = true;
    }

    if (ay != 0) {
        matrix_t ry = {{
                               {cosf(ay), 0, sinf(ay), 0},
                               {0, 1, 0, 0},
                               {-sinf(ay), 0, cosf(ay), 0},
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
                               {cosf(az), -sinf(az), 0, 0},
                               {sinf(az), cosf(az), 0, 0},
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
