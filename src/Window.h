//
// Created by Davide Caroselli on 22/09/24.
//

#ifndef BITCRAFT_WINDOW_H
#define BITCRAFT_WINDOW_H

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace bitcraft {

    class Window {
        friend class InputController;

    public:
        explicit Window(int antialiasing = 4);

        void Create(const std::string &title, GLsizei width, GLsizei height);

        void Refresh() const;

        [[nodiscard]] GLsizei GetHeight() const {
            return height;
        }

        [[nodiscard]] GLsizei GetWidth() const {
            return width;
        }

        [[nodiscard]] GLfloat GetAspectRatio() const {
            return aspectRatio;
        }

        [[nodiscard]] bool IsFullscreen() const {
            return fullscreen;
        }

        void SetFullscreen(bool value);

        [[nodiscard]] bool ShouldClose() const;

    private:
        GLsizei width = 0;
        GLsizei height = 0;
        GLfloat aspectRatio = 1.f;
        bool fullscreen = false;

        GLFWwindow *handle = nullptr;

        void OnResize(int width, int height);
    };

}

#endif //BITCRAFT_WINDOW_H
