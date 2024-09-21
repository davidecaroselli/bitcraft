//
// Created by Davide Caroselli on 21/09/24.
//

#ifndef BITCRAFT_CAMERA_H
#define BITCRAFT_CAMERA_H

#include "geometry/vertex.h"

class Camera {
public:
    Camera() : position({0, 0, 0}) {}

    explicit Camera(const vertex_t &position) : position(position) {}

    [[nodiscard]] const vertex_t &GetPosition() const {
        return position;
    }

    void SetPosition(const vertex_t &p) {
        position = p;
    }

private:
    vertex_t position;
};


#endif //BITCRAFT_CAMERA_H
