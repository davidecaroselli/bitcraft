//
// Created by Davide Caroselli on 21/09/24.
//

#include "Camera.h"

matrix_t PointAtMatrix(const vertex_t &pos, const vertex_t &target, const vertex_t &up) {
    vertex_t newForward = target - pos;
    newForward /= newForward.length();

    vertex_t a = newForward * (up * newForward);
    vertex_t newUp = up - a;
    newUp /= newUp.length();

    vertex_t newRight = newUp.cross(newForward);

    return {
            newRight.x, newRight.y, newRight.z, 0,
            newUp.x, newUp.y, newUp.z, 0,
            newForward.x, newForward.y, newForward.z, 0,
            pos.x, pos.y, pos.z, 1
    };
}

Camera::Camera() : position({0, 0, 0}),
                   forward(0, 0, -1), right(1, 0, 0), up(0, 1, 0) {
}

matrix_t Camera::GetViewMatrix() const {
    matrix_t pointAt = PointAtMatrix(position, position + forward, up);

    // Quick inverse of the pointAt matrix to get the view matrix, or "look at" matrix.
    // This inverse is possible because the matrix is a rotation and translation only matrix.
    // For a more general case, the inverse is more complex.

    vertex_t A = {pointAt(0, 0), pointAt(0, 1), pointAt(0, 2)};
    vertex_t B = {pointAt(1, 0), pointAt(1, 1), pointAt(1, 2)};
    vertex_t C = {pointAt(2, 0), pointAt(2, 1), pointAt(2, 2)};
    vertex_t T = {pointAt(3, 0), pointAt(3, 1), pointAt(3, 2)};

    return {
            A.x, B.x, C.x, 0,
            A.y, B.y, C.y, 0,
            A.z, B.z, C.z, 0,
            -(T * A), -(T * B), -(T * C), 1
    };
}

void Camera::Rotate(float tilt, float yaw, float roll) {
    matrix_t rotation = matrix_t::Rotate(tilt, yaw, roll);
    forward *= rotation;
    right *= rotation;
    up *= rotation;
}