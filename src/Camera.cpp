//
// Created by Davide Caroselli on 23/09/24.
//

#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace bitcraft;

void Camera::Rotate(float horizontal, float vertical) {
    hAngle += horizontal;
    vAngle += vertical;
}

void Camera::Move(float dx, float dy, float dz) {
    glm::vec3 direction(
            cos(vAngle) * sin(hAngle),
            sin(vAngle),
            cos(vAngle) * cos(hAngle)
    );

    glm::vec3 right(
            sin(hAngle - M_PI / 2.f),
            0,
            cos(hAngle - M_PI / 2.f)
    );

    glm::vec3 up = glm::cross(right, direction);

    position += right * dx;
    position += up * dy;
    position += direction * -dz;
}

glm::mat4 Camera::GetViewProjectionMatrix(float aspectRatio) const {
    glm::vec3 direction(
            cos(vAngle) * sin(hAngle),
            sin(vAngle),
            cos(vAngle) * cos(hAngle)
    );

    glm::vec3 right(
            sin(hAngle - M_PI / 2.f),
            0,
            cos(hAngle - M_PI / 2.f)
    );

    glm::vec3 up = glm::cross(right, direction);

    glm::mat4 viewMatrix = glm::lookAt(
            position,
            position + direction,
            up
    );

    glm::mat4 projectionMatrix = glm::perspective(
            glm::radians(fov),
            aspectRatio,
            zNear,
            zFar
    );

    return projectionMatrix * viewMatrix;
}
