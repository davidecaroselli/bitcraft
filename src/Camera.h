//
// Created by Davide Caroselli on 23/09/24.
//

#ifndef BITCRAFT_CAMERA_H
#define BITCRAFT_CAMERA_H

#include <glm/glm.hpp>
#include <cmath>

namespace bitcraft {

    class Camera {
    public:
        explicit Camera(glm::vec3 position = {0, 0, 0}, float hAngle = M_PI, float vAngle = 0,
                        float fov = 45.f, float zNear = .1f, float zFar = 100.f)
                : position(position), hAngle(hAngle), vAngle(vAngle), fov(fov), zNear(zNear), zFar(zFar) {};

        [[nodiscard]] float GetFov() const {
            return fov;
        }

        void SetFov(float v) {
            fov = v;
        }

        [[nodiscard]] float GetZNear() const {
            return zNear;
        }

        void SetZNear(float v) {
            zNear = v;
        }

        [[nodiscard]] float GetZFar() const {
            return zFar;
        }

        void SetZFar(float v) {
            zFar = v;
        }

        void Rotate(float horizontal, float vertical);

        void Move(float dx, float dy, float dz);

        [[nodiscard]] glm::mat4 GetViewProjectionMatrix(float aspectRatio) const;

    private:
        glm::vec3 position;
        float hAngle;
        float vAngle;

        float fov;
        float zNear;
        float zFar;
    };

}

#endif //BITCRAFT_CAMERA_H
