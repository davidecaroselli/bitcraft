//
// Created by Davide Caroselli on 12/09/24.
//

#ifndef ENGINE3D_ENGINE3D_H
#define ENGINE3D_ENGINE3D_H

#include <string>
#include "Screen.h"
#include "geometry/face_t.h"

struct Color {
    float r, g, b;
};

class DebugInfoView {
public:
    explicit DebugInfoView(float updateSecs = 0.5f) : updateSecs(updateSecs) {}

    void OnRender(float elapsedTime);

    void DrawToScreen(Screen *screen) const;

private:
    const float updateSecs;
    float tElapsedTime = 0;
    int renderCount = 0;

    float fps = 0;
};

class Engine3D {
    friend class ScreenCallback;

public:
    explicit Engine3D(std::string name);

    [[nodiscard]] const Screen *GetScreen() const {
        return screen;
    }

    [[nodiscard]] bool ShowDebugInfo() const {
        return showDebugInfo;
    }

    void SetShowDebugInfo(bool show) {
        showDebugInfo = show;
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

    virtual bool Render(float elapsedTime) = 0;

    // Draw functions ---------------------------------------------------

    void ClearScreen(const Color &color);

    void DrawTriangle(const face_t &triangle, const Color &color);

    void FillTriangle(const face_t &triangle, const Color &color);

protected:
    const std::string name;
    Screen *screen;

    DebugInfoView debugView;
    bool showDebugInfo = false;
    float fov = 90;
    float zFar = 1000;
    float zNear = 0.1;

    matrix_t prjMatrix = {0};

    void OnRender(float elapsedTime);

    void ComputeProjectionMatrix();
};


#endif //ENGINE3D_ENGINE3D_H
