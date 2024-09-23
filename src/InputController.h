//
// Created by Davide Caroselli on 22/09/24.
//

#ifndef BITCRAFT_INPUTCONTROLLER_H
#define BITCRAFT_INPUTCONTROLLER_H

#include "Window.h"

namespace bitcraft {

    struct movement_t {
        float horizontal;
        float vertical;
    };

    class InputController {
    public:
        explicit InputController(float sensitivity = 0.4f, float speed = 6.0f)
                : sensitivity(sensitivity), speed(speed) {};

        [[nodiscard]] float GetSensitivity() const {
            return sensitivity;
        }

        void SetSensitivity(float v) {
            sensitivity = v;
        }

        [[nodiscard]] float GetSpeed() const {
            return speed;
        }

        void SetSpeed(float v) {
            speed = v;
        }

        void Attach(const Window &window);

        bool IsKeyPressed(int key);

        movement_t GetMouseMovement();

        movement_t GetWASDMovement();

    private:
        float sensitivity = 0.005f;
        float speed = 3.0f;

        Window *window = nullptr;

        bool mouseInitialized = false;
        movement_t lastMousePos = {0, 0};
    };
}

#endif //BITCRAFT_INPUTCONTROLLER_H
