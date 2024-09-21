//
// Created by Davide Caroselli on 12/09/24.
//

#include "GameEngine.h"
#include <iostream>
#include <iomanip>
#include <GLUT/glut.h>
#include <sstream>

// Draw functions ---------------------------------------------------

void DrawTriangle(const face_t &triangle, const color_t &color) {
    glBegin(GL_LINE_LOOP);
    glColor3f(color.r, color.g, color.b);
    for (const auto &v: triangle.vs)
        glVertex2f(v.x, v.y);
    glEnd();
}

void FillTriangle(const face_t &triangle, const color_t &color) {
    glBegin(GL_TRIANGLES);
    glColor3f(color.r, color.g, color.b);
    for (const auto &v: triangle.vs)
        glVertex2f(v.x, v.y);
    glEnd();
}

// Utility classes --------------------------------------------------

class ScreenCallback : public IScreenCallback {
public:
    explicit ScreenCallback(GameEngine *engine) : engine(engine) {}

    void OnRender(float elapsedTime) override {
        engine->OnRender(elapsedTime);
    }

    void OnResize() override {
        engine->ComputeProjectionMatrix();
    }

private:
    GameEngine *engine;
};

void DebugInfoView::OnRender(float elapsedTime) {
    renderCount++;
    tElapsedTime += elapsedTime;

    if (tElapsedTime >= updateSecs) {
        fps = (float) renderCount / tElapsedTime;
        renderCount = 0;
        tElapsedTime = 0;
    }
}

void DebugInfoView::DrawToScreen(Screen &screen) const {
    std::stringstream stream;
    stream << "FPS: " << std::fixed << std::setprecision(1) << fps;
    std::string fpsLog = stream.str();

    float glScaleX = 2.f / (float) screen.GetWidth();
    float glScaleY = 2.f / (float) screen.GetHeight();
    float labelWidth = 8 * (float) fpsLog.size();

    glColor3f(0, 0, 0);
    glRectf(-1, -1, (8 + labelWidth + 8) * glScaleX - 1, 20 * glScaleY - 1);

    glColor3f(1, 1, 1);
    glRasterPos2f(8 * glScaleX - 1, 6 * glScaleY - 1);
    for (char c: fpsLog)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
}

// Game engine ------------------------------------------------------

GameEngine::GameEngine(std::string name) : name(std::move(name)), screen(new ScreenCallback(this)) {
}

void GameEngine::Start(unsigned int width, unsigned int height, unsigned int fps, bool fullscreen) {
    screen.Init(name, width, height, fps);
    screen.SetFullscreen(fullscreen);

    input.Init();

    ComputeProjectionMatrix();
    glutMainLoop();
}

void GameEngine::ComputeProjectionMatrix() {
    auto a = screen.GetAspectRatio();
    auto f = (float) (1. / tan(fov * 0.5 * M_PI / 180.));
    auto q = zFar / (zFar - zNear);

    this->prjMatrix = {
            a * f, 0, 0, 0,
            0, f, 0, 0,
            0, 0, q, 1,
            0, 0, -zNear * q, 0
    };
}

void GameEngine::OnRender(float elapsedTime) {
    debugView.OnRender(elapsedTime);

    // Clear screen
    glClearColor(clearColor.r, clearColor.g, clearColor.b, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update scene
    this->OnUpdate(elapsedTime);

    // Collect and light visible faces
    const vertex_t &cameraPos = camera.GetPosition();
    const vertex_t &lightDirection = light.GetDirection();

    std::vector<face_t> faces;
    faces.reserve(scene.size());

    for (auto &face: scene) {
        vertex_t normal = face.normal();

        if (normal * (face[0] - cameraPos) < 0) {
            float color = normal * lightDirection;
            face.color = {color, color, color};

            faces.push_back(face);
        }
    }

    // Apply camera matrix
    matrix_t viewMatrix = camera.GetViewMatrix();
    for (auto &face: faces)
        face *= viewMatrix;

    // Apply projection matrix
    for (auto &face: faces)
        face *= prjMatrix;

    // Rasterize scene
    std::sort(faces.begin(), faces.end(), [](const face_t &a, const face_t &b) {
        float az = (a[0].z + a[1].z + a[2].z) / 3.f;
        float bz = (b[0].z + b[1].z + b[2].z) / 3.f;
        return az > bz;
    });

    for (const auto &face: faces) {
        FillTriangle(face, face.color);
        if (wireframe)
            DrawTriangle(face, {1, 1, 1});
    }

    // Draw debug info if enabled
    if (showDebugInfo)
        debugView.DrawToScreen(screen);
}
