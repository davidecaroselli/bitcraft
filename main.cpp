#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>
#include "Window.h"
#include "core/Shaders.h"
#include "InputController.h"
#include "core/Texture.h"
#include "Camera.h"
#include "Model.h"
#include <memory>

using namespace glm;
namespace bc = bitcraft;
//
//// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
//// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
//const int triCount = 12;
//static const GLfloat g_vertex_buffer_data[] = {
//        0, 0, 0, 0, 1, 0, 1, 1, 0,
//        0, 0, 0, 1, 1, 0, 1, 0, 0, // front
//
//        0, 0, -1, 1, 1, -1, 0, 1, -1,
//        0, 0, -1, 1, 0, -1, 1, 1, -1, // back
//
//        0, 0, 0, 0, 0, -1, 0, 1, 0,
//        0, 0, -1, 0, 1, -1, 0, 1, 0, // left
//
//        1, 0, 0, 1, 1, 0, 1, 0, -1,
//        1, 0, -1, 1, 1, 0, 1, 1, -1, // right
//
//        0, 1, 0, 0, 1, -1, 1, 1, -1,
//        0, 1, 0, 1, 1, -1, 1, 1, 0, // top
//
//        0, 0, 0, 1, 0, -1, 0, 0, -1,
//        0, 0, 0, 1, 0, 0, 1, 0, -1,  // bottom
//};
//
//// Two UV coordinatesfor each vertex. They were created with Blender.
//static const GLfloat g_uv_buffer_data[] = {
//        0, 0, 0, 1, 1, 1,
//        0, 0, 1, 1, 1, 0,  // front
//
//        0, 0, 1, 1, 0, 1,
//        0, 0, 1, 0, 1, 1,  // back
//
//        0, 0, 1, 0, 0, 1,
//        0, 0, 0, 1, 1, 1,  // left
//
//        0, 0, 0, 1, 1, 0,
//        1, 0, 0, 1, 1, 1, // right
//
//        0, 0, 0, 1, 1, 1,
//        0, 0, 1, 1, 1, 0, // top
//
//        0, 0, 1, 1, 0, 1,
//        0, 0, 1, 0, 1, 1, // bottom
//};
//
//
GLuint vertexbuffer;
GLuint uvbuffer;

int main() {
    bc::Window screen(4);
    screen.Create("Bitcraft", 1024, 768);

    bc::InputController controller;
    controller.Attach(screen);

    bc::Camera camera({0, 0, 5});

    bc::Model model("models/Aridia.obj");

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    bc::Shaders::Loader loader;
    loader.LoadBuiltinShader("DefaultVertexShader.glsl", GL_VERTEX_SHADER);
    loader.LoadBuiltinShader("DefaultFragmentShader.glsl", GL_FRAGMENT_SHADER);
    std::shared_ptr<bc::Shaders> shaders(loader.Build());

    bc::Texture texture("textures/High.png");
    GLuint Texture = texture.id;
    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureID = shaders->NewVariable("myTextureSampler");

    // This will identify our vertex buffer
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(glm::vec3), &model.vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, model.uvs.size() * sizeof(glm::vec2), &model.uvs[0], GL_STATIC_DRAW);

    GLuint MatrixID = shaders->NewVariable("MVP");

    glFrontFace(GL_CCW);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    double lastTime = glfwGetTime();

    do {
        // Use our shader
        shaders->Use();

        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double currentTime = glfwGetTime();
        auto deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;

        float dy = 0;
        if (controller.IsKeyPressed(GLFW_KEY_K)) dy += controller.GetSpeed() / 2.f;
        if (controller.IsKeyPressed(GLFW_KEY_M)) dy -= controller.GetSpeed() / 2.f;

        bc::movement_t mouse = controller.GetMouseMovement();
        bc::movement_t wasd = controller.GetWASDMovement();
        camera.Rotate(-mouse.horizontal * deltaTime, 0);
        camera.Move(wasd.horizontal * deltaTime, dy * deltaTime, -wasd.vertical * deltaTime);

        glm::mat4 MVP = camera.GetViewProjectionMatrix(screen.GetAspectRatio());
        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(TextureID, 0);

        // draw model
        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void *) 0            // array buffer offset
        );

        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(
                1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                2,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void *) 0                          // array buffer offset
        );

// Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, model.vertices.size()); // Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Swap buffers
        screen.Refresh();
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (!controller.IsKeyPressed(GLFW_KEY_ESCAPE) && !screen.ShouldClose());

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
//    glDeleteProgram(programID);
    glDeleteTextures(1, &Texture);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
