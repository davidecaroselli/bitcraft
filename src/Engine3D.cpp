//
// Created by Davide Caroselli on 12/09/24.
//

#include "Engine3D.h"
#include <iostream>
#include <iomanip>
#include <GLUT/glut.h>
#include <unistd.h>
#include <sys/time.h>
#include <sstream>

class ScreenCallback : public IScreenCallback {
public:
    explicit ScreenCallback(Engine3D *engine) : engine(engine) {}

    void OnRender(float elapsedTime) override {
        engine->OnRender(elapsedTime);
    }

    void OnResize() override {
        engine->ComputeProjectionMatrix();
    }

private:
    Engine3D *engine;
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

void DebugInfoView::DrawToScreen(Screen *screen) const {
    std::stringstream stream;
    stream << "FPS: " << std::fixed << std::setprecision(1) << fps;
    std::string fpsLog = stream.str();

    float glScaleX = 2.f / (float) screen->GetWidth();
    float glScaleY = 2.f / (float) screen->GetHeight();
    float labelWidth = 8 * (float) fpsLog.size();

    glColor3f(0, 0, 0);
    glRectf(-1, -1, (8 + labelWidth + 8) * glScaleX - 1, 20 * glScaleY - 1);

    glColor3f(1, 1, 1);
    glRasterPos2f(8 * glScaleX - 1, 6 * glScaleY - 1);
    for (char c: fpsLog)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
}

Engine3D::Engine3D(std::string name) : name(std::move(name)), screen(Screen::GetInstance()),
                                       input(InputController::GetInstance()) {
    auto *callback = new ScreenCallback(this);
    screen->SetCallback(callback);
}

void Engine3D::Start(unsigned int width, unsigned int height, unsigned int fps, bool fullscreen) {
    screen->Init(name, width, height, fps);
    screen->SetFullscreen(fullscreen);

    input->Init();

    ComputeProjectionMatrix();
    glutMainLoop();
}

void Engine3D::ComputeProjectionMatrix() {
    auto a = screen->GetAspectRatio();
    auto f = (float) (1. / tan(fov * 0.5 * M_PI / 180.));
    auto q = zFar / (zFar - zNear);

    this->prjMatrix = {
            a * f, 0, 0, 0,
            0, f, 0, 0,
            0, 0, q, 1,
            0, 0, -zNear * q, 0
    };
}

void Engine3D::OnRender(float elapsedTime) {
    debugView.OnRender(elapsedTime);

    this->Render(elapsedTime);

    if (showDebugInfo)
        debugView.DrawToScreen(screen);
}

// Draw functions ---------------------------------------------------

void Engine3D::ClearScreen(const color_t &color) {
    glClearColor(color.r, color.g, color.b, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Engine3D::DrawTriangle(const face_t &triangle, const color_t &color) {
    glBegin(GL_LINE_LOOP);
    glColor3f(color.r, color.g, color.b);
    for (const auto &v: triangle.vs)
        glVertex2f(v.x, v.y);
    glEnd();
}

void Engine3D::FillTriangle(const face_t &triangle, const color_t &color) {
    glBegin(GL_TRIANGLES);
    glColor3f(color.r, color.g, color.b);
    for (const auto &v: triangle.vs)
        glVertex2f(v.x, v.y);
    glEnd();
}
