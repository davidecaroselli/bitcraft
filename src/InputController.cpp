//
// Created by Davide Caroselli on 17/09/24.
//

#include "InputController.h"
#include <GLUT/glut.h>
#include <iostream>

InputController *InputController::instance = nullptr;

InputController *InputController::GetInstance() {
    if (!instance)
        instance = new InputController();

    return instance;
}

InputController::InputController() {
    for (bool &key: keys)
        key = false;
}

void InputController::Init() {
    glutKeyboardFunc([](unsigned char key, int x, int y) {
        InputController::GetInstance()->keys[key] = true;
    });
    glutKeyboardUpFunc([](unsigned char key, int x, int y) {
        InputController::GetInstance()->keys[key] = false;
    });
}
