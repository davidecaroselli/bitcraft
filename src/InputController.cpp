//
// Created by Davide Caroselli on 22/09/24.
//

#include "InputController.h"

using namespace bitcraft;

void InputController::Attach(const Window &window) {
    if (window.handle == nullptr)
        throw std::runtime_error("Window has not been created yet");

    wHandle = window.handle;
    glfwSetInputMode(wHandle, GLFW_STICKY_KEYS, GL_TRUE);
}

bool InputController::IsKeyPressed(int key) {
    return wHandle && glfwGetKey(wHandle, key) == GLFW_PRESS;
}
