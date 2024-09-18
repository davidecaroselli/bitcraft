//
// Created by Davide Caroselli on 17/09/24.
//

#ifndef BITCRAFT_INPUTCONTROLLER_H
#define BITCRAFT_INPUTCONTROLLER_H


class InputController {
public:
    static InputController *GetInstance();

    void Init();

    bool IsKeyPressed(unsigned char key) {
        return keys[key];
    }

private:
    static InputController *instance;

    bool keys[256]{};

    InputController();
};


#endif //BITCRAFT_INPUTCONTROLLER_H
