//
// Created by Davide Caroselli on 16/09/24.
//

#ifndef BITCRAFT_SCREEN_H
#define BITCRAFT_SCREEN_H

#include <string>
#include <vector>
#include <unistd.h>
#include "geometry/plane.h"

class IScreenCallback {
public:
    virtual void OnRender(float elapsedTime) = 0;

    virtual void OnResize() = 0;
};

class Screen {
public:
    const std::vector<plane_t> bounds;

    explicit Screen(IScreenCallback *cb);

    [[nodiscard]] unsigned int GetHeight() const {
        return height;
    }

    [[nodiscard]] unsigned int GetWidth() const {
        return width;
    }

    [[nodiscard]] float GetAspectRatio() const {
        return aspectRatio;
    }

    [[nodiscard]] unsigned int GetFPS() const {
        return fps;
    }

    void SetFPS(unsigned int value) {
        fps = value;
    }

    void Init(const std::string &title, unsigned int width, unsigned int height, unsigned int fps = 60);

    [[nodiscard]] bool IsFullscreen() const {
        return fullscreen;
    }

    void SetFullscreen(bool value);

private:
    unsigned int width = 0;
    unsigned int height = 0;
    float aspectRatio = 1;

    unsigned int fps = 60;
    bool fullscreen = false;
    IScreenCallback *callback;
    uint64_t lastRenderTimestamp = 0;

    void OnIdle();

    void OnResize(unsigned int w, unsigned int h);

    void OnRender();

    uint64_t GetTimeToNextRender(uint64_t *timestamp = nullptr);
};


#endif //BITCRAFT_SCREEN_H
