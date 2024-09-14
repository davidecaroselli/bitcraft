//
// Created by Davide Caroselli on 12/09/24.
//

#ifndef ENGINE3D_ENGINE3D_H
#define ENGINE3D_ENGINE3D_H

#include <string>
#include "algebra3d.h"

struct Color {
    float r, g, b;
};

class Engine3D {
public:
    Engine3D(std::string name, unsigned int width, unsigned int height, unsigned int fps = 60);

    [[nodiscard]] unsigned int GetWidth() const {
        return width;
    }

    [[nodiscard]] unsigned int GetHeight() const {
        return height;
    }

    [[nodiscard]] float GetAspectRatio() const {
        return aspectRatio;
    }

    [[nodiscard]] unsigned int GetFPS() const {
        return fps;
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

    void Start(bool fullscreen = false);

    virtual bool Render(float elapsedTime) = 0;

    // Draw functions ---------------------------------------------------

    void ClearScreen(const Color &color);

    void DrawTriangle(const triangle_t &triangle, const Color &color);

    void FillTriangle(const triangle_t &triangle, const Color &color);

protected:
    const std::string name;
    const unsigned int width;
    const unsigned int height;
    const float aspectRatio;
    const unsigned int fps;

    bool showDebugInfo = false;
    float fov = 90;
    float zFar = 1000;
    float zNear = 0.1;

    matrix_t prjMatrix = {0};

    void ComputeProjectionMatrix();
};


#endif //ENGINE3D_ENGINE3D_H
