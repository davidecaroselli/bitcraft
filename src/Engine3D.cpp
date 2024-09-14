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

Engine3D *GlobalInstance = nullptr;
float glScaleX = 0.f;
float glScaleY = 0.f;
uint64_t LastRenderTimestamp = 0;

uint64_t GetTimeToNextRender(uint64_t *timestamp = nullptr) {
    struct timeval tv{};
    gettimeofday(&tv, nullptr);
    uint64_t currentTimestamp = tv.tv_sec * (uint64_t) 1000000 + tv.tv_usec;

    if (LastRenderTimestamp == 0)
        LastRenderTimestamp = currentTimestamp;

    if (timestamp != nullptr)
        *timestamp = currentTimestamp;

    unsigned int fps = GlobalInstance ? GlobalInstance->GetFPS() : 60;
    int timeBetweenRender = (int) (1000000.f / (float) fps);

    uint64_t elapsedTime = currentTimestamp - LastRenderTimestamp;
    if (elapsedTime < timeBetweenRender)
        return timeBetweenRender - elapsedTime;

    return 0;
}

struct FPSCounter {
    int renderCount = 0;
    uint64_t lastTimestamp = 0;
    float value = 0;

    void Draw() const {
        std::stringstream stream;
        stream << "FPS: " << std::fixed << std::setprecision(1) << value;
        std::string fpsLog = stream.str();

        float labelWidth = 8 * (float) fpsLog.size();

        glColor3f(0, 0, 0);
        glRectf(-1, -1, (8 + labelWidth + 8) * glScaleX - 1, 20 * glScaleY - 1);

        glColor3f(1, 1, 1);
        glRasterPos2f(8 * glScaleX - 1, 6 * glScaleY - 1);
        for (char c: fpsLog)
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
    }
} FpsCounter;

void render() {
    if (GlobalInstance == nullptr) return;

    GLint m_viewport[4];
    glGetIntegerv(GL_VIEWPORT, m_viewport);
    glScaleX = 2.f / (float) m_viewport[2];
    glScaleY = 2.f / (float) m_viewport[3];

    uint64_t currentTimestamp;
    // Check if render has been called too soon
    if (GetTimeToNextRender(&currentTimestamp) > 0) return;

    float deltaTime = (float) (currentTimestamp - LastRenderTimestamp) / 1000000.f;
    LastRenderTimestamp = currentTimestamp;

    // Count FPS
    if (FpsCounter.lastTimestamp == 0) {
        FpsCounter.lastTimestamp = currentTimestamp;
    } else {
        float elapsed = (float) (currentTimestamp - FpsCounter.lastTimestamp) / 1000000.f;
        if (elapsed >= .5f) {
            FpsCounter.value = (float) FpsCounter.renderCount / elapsed;
            FpsCounter.renderCount = 0;
            FpsCounter.lastTimestamp = currentTimestamp;
        }
    }

    // Rendering
    GlobalInstance->Render(deltaTime);

    // Show FPS debug log
    if (GlobalInstance->ShowDebugInfo() && FpsCounter.value > 0)
        FpsCounter.Draw();

    glutSwapBuffers();

    FpsCounter.renderCount++;
}

void idle() {
    int waitTime = (int) ((double) GetTimeToNextRender() * .80);
    if (waitTime > 5000)
        usleep(waitTime);

    glutPostRedisplay();
}

Engine3D::Engine3D(std::string name, unsigned int width, unsigned int height, unsigned int fps)
        : name(std::move(name)), width(width), height(height), aspectRatio((float) height / (float) width), fps(fps) {
    if (GlobalInstance != nullptr)
        throw std::runtime_error("Only one instance of Engine3D is allowed");

    GlobalInstance = this;
    ComputeProjectionMatrix();
}

void Engine3D::Start(bool fullscreen) {
    int argc = 0;
    char *argv[0];

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize((int) width, (int) height);
    glutCreateWindow(name.c_str());

    if (fullscreen)
        glutFullScreen();

    glutDisplayFunc(render);
    glutIdleFunc(idle);

    glutMainLoop();
}

void Engine3D::ComputeProjectionMatrix() {
    auto a = aspectRatio;
    auto f = (float) (1. / tan(fov * 0.5 * M_PI / 180.));
    auto q = zFar / (zFar - zNear);

    this->prjMatrix = {
            a * f, 0, 0, 0,
            0, f, 0, 0,
            0, 0, q, 1,
            0, 0, - zNear * q, 0
    };
}

// Draw functions ---------------------------------------------------

void Engine3D::ClearScreen(const Color &color) {
    glClearColor(color.r, color.g, color.b, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Engine3D::DrawTriangle(const triangle_t &triangle, const Color &color) {
    triangle_t prjTri = triangle * prjMatrix;

    glBegin(GL_LINE_LOOP);
    glColor3f(color.r, color.g, color.b);
    for (const auto &v: prjTri.v)
        glVertex2f(v.x, v.y);
    glEnd();
}

void Engine3D::FillTriangle(const triangle_t &triangle, const Color &color) {
    triangle_t prjTri = triangle * prjMatrix;

    glBegin(GL_TRIANGLES);
    glColor3f(color.r, color.g, color.b);
    for (const auto &v: prjTri.v)
        glVertex2f(v.x, v.y);
    glEnd();
}
