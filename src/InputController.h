//
// Created by Davide Caroselli on 22/09/24.
//

#ifndef BITCRAFT_INPUTCONTROLLER_H
#define BITCRAFT_INPUTCONTROLLER_H

#include "Window.h"

namespace bitcraft {
    class InputController {
    public:
        void Attach(const Window &window);

        bool IsKeyPressed(int key);

    private:
        GLFWwindow *wHandle = nullptr;
    };
}

#endif //BITCRAFT_INPUTCONTROLLER_H
