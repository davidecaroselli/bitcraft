//
// Created by Davide Caroselli on 17/09/24.
//

#include "InputController.h"
#include <GLUT/glut.h>
#include <iostream>

static InputController *instance = nullptr;

InputController::InputController() {
    if (instance)
        throw std::runtime_error("InputController instance already exists");
    instance = this;

    for (bool &key: keys)
        key = false;
}

void InputController::Init() {
    glutKeyboardFunc([](unsigned char key, int x, int y) {
        instance->keys[key] = true;
    });
    glutKeyboardUpFunc([](unsigned char key, int x, int y) {
        instance->keys[key] = false;
    });
}
