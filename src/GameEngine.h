//
// Created by Davide Caroselli on 12/09/24.
//

#ifndef BITCRAFT_GAMEENGINE_H
#define BITCRAFT_GAMEENGINE_H

#include <string>
#include <vector>
#include "geometry/color.h"
#include "geometry/face.h"
#include "Screen.h"
#include "InputController.h"
#include "Camera.h"
#include "LightSource.h"

class DebugInfoView {
public:
    explicit DebugInfoView(float updateSecs = 0.5f) : updateSecs(updateSecs) {}

    void OnRender(float elapsedTime);

    void DrawToScreen(Screen &screen) const;

private:
    const float updateSecs;
    float tElapsedTime = 0;
    int renderCount = 0;

    float fps = 0;
};

class GameEngine {
    friend class ScreenCallback;

public:
    Screen screen;
    InputController input;
    Camera camera;
    LightSource light;

    std::vector<face_t> scene;

    explicit GameEngine(std::string name);

    void SetClearColor(const color_t &color) {
        clearColor = color;
    }

    [[nodiscard]] bool IsDebugInfoVisible() const {
        return showDebugInfo;
    }

    void ShowDebugInfo() {
        showDebugInfo = true;
    }

    void HideDebugInfo() {
        showDebugInfo = false;
    }

    [[nodiscard]] bool IsWireframeVisible() const {
        return wireframe;
    }

    void SetWireframe(bool show) {
        wireframe = show;
    }

    [[nodiscard]] float GetFOV() const {
        return fov;
    }

    void SetFOV(float value) {
        fov = value;
        ComputeProjectionMatrix();
    }

    [[nodiscard]] float GetZNear() const {
        return zNear;
    }

    [[nodiscard]] float GetZFar() const {
        return zNear;
    }

    void SetDrawDistance(float near, float far) {
        zNear = near;
        zFar = far;
        ComputeProjectionMatrix();
    }

    void Start(unsigned int width, unsigned int height, unsigned int fps = 60, bool fullscreen = false);

    virtual bool OnUpdate(float elapsedTime) = 0;

protected:
    const std::string name;

    DebugInfoView debugView;
    bool showDebugInfo = false;
    bool wireframe = false;
    float fov = 90;
    float zFar = 1000;
    float zNear = 0.1;

    matrix_t prjMatrix = {0};
    color_t clearColor = {0, 0, 0};

    void OnRender(float elapsedTime);

    void ComputeProjectionMatrix();
};


#endif //BITCRAFT_GAMEENGINE_H
