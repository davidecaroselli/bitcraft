//
// Created by Davide Caroselli on 16/09/24.
//

#include "Screen.h"
#include <GLUT/glut.h>
#include <sys/time.h>

static Screen *instance = nullptr;

Screen::Screen(IScreenCallback *cb) : callback(cb) {
    if (instance != nullptr)
        throw std::runtime_error("Screen instance already exists");
    instance = this;
}

void Screen::Init(const std::string &title, unsigned int width, unsigned int height, unsigned int fps) {
    this->fps = fps;

    int argc = 0;
    char *argv[0];
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize((int) width, (int) height);
    glutCreateWindow(title.c_str());

    glutDisplayFunc([]() {
        if (instance != nullptr)
            instance->OnRender();
    });
    glutReshapeFunc([](int w, int h) {
        glViewport(0, 0, w, h);

        if (instance != nullptr)
            instance->OnResize(w, h);
    });
    glutIdleFunc([]() {
        if (instance != nullptr)
            instance->OnIdle();
    });
}

void Screen::SetFullscreen(bool value) {
    fullscreen = value;
    if (fullscreen) {
        glutFullScreen();
    } else {
        int w = (int) GetWidth();
        int h = (int) GetHeight();
        if (w > 0 && h > 0)
            glutReshapeWindow((int) w, (int) h);
    }
}

void Screen::OnIdle() {
    int waitTime = (int) ((double) GetTimeToNextRender() * .80);
    if (waitTime > 5000)
        usleep(waitTime);

    glutPostRedisplay();
}

void Screen::OnResize(unsigned int w, unsigned int h) {
    width = (unsigned int) w;
    height = (unsigned int) h;
    aspectRatio = (float) h / (float) w;

    if (callback != nullptr)
        callback->OnResize();
}

void Screen::OnRender() {
    uint64_t currentTimestamp;
    // Check if render has been called too soon
    if (GetTimeToNextRender(&currentTimestamp) > 0) return;

    float deltaTime = (float) (currentTimestamp - lastRenderTimestamp) / 1000000.f;
    lastRenderTimestamp = currentTimestamp;

    // Rendering
    if (callback != nullptr)
        callback->OnRender(deltaTime);

    glutSwapBuffers();
}

uint64_t Screen::GetTimeToNextRender(uint64_t *timestamp) {
    struct timeval tv{};
    gettimeofday(&tv, nullptr);
    uint64_t currentTimestamp = tv.tv_sec * (uint64_t) 1000000 + tv.tv_usec;

    if (lastRenderTimestamp == 0)
        lastRenderTimestamp = currentTimestamp;

    if (timestamp != nullptr)
        *timestamp = currentTimestamp;

    int timeBetweenRender = (int) (1000000.f / (float) fps);

    uint64_t elapsedTime = currentTimestamp - lastRenderTimestamp;
    if (elapsedTime < timeBetweenRender)
        return timeBetweenRender - elapsedTime;

    return 0;
}
