//
// Created by Davide Caroselli on 22/09/24.
//

#include "InputController.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace bitcraft;

void InputController::Attach(const Window &w) {
    if (w.handle == nullptr)
        throw std::runtime_error("Window has not been created yet");

    window = (Window *) &w;
    glfwSetInputMode(window->handle, GLFW_STICKY_KEYS, GL_TRUE);
}

bool InputController::IsKeyPressed(int key) {
    return window && window->handle && glfwGetKey(window->handle, key) == GLFW_PRESS;
}

movement_t InputController::GetMouseMovement() {
    float dx = 0, dy = 0;
    if (IsKeyPressed(GLFW_KEY_A))
        dx -= sensitivity;
    if (IsKeyPressed(GLFW_KEY_D))
        dx += sensitivity;

    return {dx, dy};

//    float nx = (float) window->GetWidth() / 2.f;
//    float ny = (float) window->GetHeight() / 2.f;
//
//    double cx, cy;
//    glfwGetCursorPos(window->handle, &cx, &cy);
//
//    if (!mouseInitialized && cx == nx && cy == ny)
//        mouseInitialized = true;
//
//    double dx = 0, dy = 0;
//    if (mouseInitialized) {
//        dx = cx - lastMousePos.horizontal;
//        dy = lastMousePos.vertical - cy;
//    }
//
//    glfwSetCursorPos(window->handle, nx, ny);
//    lastMousePos = {nx, ny};
//
//    return {(float) dx * sensitivity, (float) dy * sensitivity};
}

movement_t InputController::GetWASDMovement() {
    float dx = 0, dy = 0;

    if (IsKeyPressed(GLFW_KEY_W))
        dy += speed;
    if (IsKeyPressed(GLFW_KEY_S))
        dy -= speed;

    return {dx, dy};
}
