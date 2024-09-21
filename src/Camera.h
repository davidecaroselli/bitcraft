//
// Created by Davide Caroselli on 21/09/24.
//

#ifndef BITCRAFT_CAMERA_H
#define BITCRAFT_CAMERA_H

#include "geometry/vertex.h"
#include "geometry/matrix.h"

/**
 * Returns a transformation matrix that "points" the object in position `pos` towards `target` with the `up` direction.
 * This can be used to create a camera matrix or, for example, make and object look and "follow" another object.
 *
 * @param pos the position of the object
 * @param target the position of the target to follow
 * @param up the up direction
 * @return a transformation matrix that points the object towards the target
 */
matrix_t PointAtMatrix(const vertex_t &pos, const vertex_t &target, const vertex_t &up);

class Camera {
public:
    Camera();

    inline void MoveForward(float delta) {
        position += forward * delta;
    }

    inline void MoveBackward(float delta) {
        MoveForward(-delta);
    }

    inline void MoveRight(float delta) {
        position += right * delta;
    }

    inline void MoveLeft(float delta) {
        MoveRight(-delta);
    }

    inline void MoveUp(float delta) {
        position += up * delta;
    }

    inline void MoveDown(float delta) {
        MoveUp(-delta);
    }

    void Rotate(float tilt, float yaw, float roll);

    [[nodiscard]] const vertex_t &GetPosition() const {
        return position;
    }

    [[nodiscard]] matrix_t GetViewMatrix() const;

private:
    vertex_t position;
    vertex_t forward;
    vertex_t right;
    vertex_t up;
};


#endif //BITCRAFT_CAMERA_H
