//
// Created by Davide Caroselli on 22/09/24.
//

#include "Window.h"

using namespace bitcraft;

static Window *instance = nullptr;

Window::Window(int antialiasing) {
    if (instance)
        throw std::runtime_error("Window instance already exists");
    instance = this;

    // Initialise GLFW
    glewExperimental = true;
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    glfwWindowHint(GLFW_SAMPLES, antialiasing);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Window::Create(const std::string &title, GLsizei w, GLsizei h) {
    OnResize(w, h);

    handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (handle == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. "
                                 "Try the 2.1 version of the tutorials.");
    }

    glfwMakeContextCurrent(handle); // Initialize GLEW
    glfwSetFramebufferSizeCallback(handle, [](GLFWwindow *window, int width, int height) {
        if (instance) {
            instance->OnResize(width, height);
            glViewport(0, 0, width, height);
        }
    });

    glewExperimental = true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLEW");
    }
}

void Window::Refresh() const {
    glfwSwapBuffers(handle);
}

void Window::OnResize(int w, int h) {
    width = w;
    height = h;
    aspectRatio = (float) width / (float) height;
}

bool Window::ShouldClose() const {
    return glfwWindowShouldClose(handle);
}
