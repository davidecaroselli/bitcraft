//
// Created by Davide Caroselli on 17/09/24.
//

#ifndef BITCRAFT_INPUTCONTROLLER_H
#define BITCRAFT_INPUTCONTROLLER_H


class InputController {
public:
    InputController();

    void Init();

    bool IsKeyPressed(unsigned char key) {
        return keys[key];
    }

private:
    bool keys[256]{};
};


#endif //BITCRAFT_INPUTCONTROLLER_H
