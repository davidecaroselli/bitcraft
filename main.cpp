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
#include <memory>

using namespace glm;
namespace bc = bitcraft;

// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
const int triCount = 12;
static const GLfloat g_vertex_buffer_data[] = {
        0, 0, 0, 0, 1, 0, 1, 1, 0,
        0, 0, 0, 1, 1, 0, 1, 0, 0, // front

        0, 0, -1, 1, 1, -1, 0, 1, -1,
        0, 0, -1, 1, 0, -1, 1, 1, -1, // back

        0, 0, 0, 0, 0, -1, 0, 1, 0,
        0, 0, -1, 0, 1, -1, 0, 1, 0, // left

        1, 0, 0, 1, 1, 0, 1, 0, -1,
        1, 0, -1, 1, 1, 0, 1, 1, -1, // right

        0, 1, 0, 0, 1, -1, 1, 1, -1,
        0, 1, 0, 1, 1, -1, 1, 1, 0, // top

        0, 0, 0, 1, 0, -1, 0, 0, -1,
        0, 0, 0, 1, 0, 0, 1, 0, -1,  // bottom
};

// Two UV coordinatesfor each vertex. They were created with Blender.
static const GLfloat g_uv_buffer_data[] = {
        0, 0, 0, 1, 1, 1,
        0, 0, 1, 1, 1, 0,  // front

        0, 0, 1, 1, 0, 1,
        0, 0, 1, 0, 1, 1,  // back

        0, 0, 1, 0, 0, 1,
        0, 0, 0, 1, 1, 1,  // left

        0, 0, 0, 1, 1, 0,
        1, 0, 0, 1, 1, 1, // right

        0, 0, 0, 1, 1, 1,
        0, 0, 1, 1, 1, 0, // top

        0, 0, 1, 1, 0, 1,
        0, 0, 1, 0, 1, 1, // bottom
};


GLuint vertexbuffer;
GLuint uvbuffer;

void Draw() {
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
    glDrawArrays(GL_TRIANGLES, 0, triCount * 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

glm::mat4 get_mvp_matrix(const bc::Window &window, float zOffset) {
    // Projection matrix: 45° Field of View, 4:3 ratio, display range: 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), window.GetAspectRatio(), 0.1f, 100.0f);

    // Camera matrix
    glm::mat4 View = glm::lookAt(
            glm::vec3(4, 3, zOffset), // Camera is at (4,3,3), in World Space
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Model matrix: an identity matrix (model will be at the origin)
    auto Model =
            glm::rotate(glm::mat4(1.0f), glm::radians(zOffset * 360), glm::vec3(0, 1, 0))
            * glm::rotate(glm::mat4(1.0f), glm::radians((1 - zOffset) * 360), glm::vec3(1, 0, 0));
    // Our ModelViewProjection: multiplication of our 3 matrices
    return Projection * View * Model; // Remember, matrix multiplication is the other way around
}

int main() {
    bc::Window screen(4);
    screen.Create("Bitcraft", 1024, 768);

    bc::InputController controller;
    controller.Attach(screen);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    bc::Shaders::Loader loader;
    loader.LoadBuiltinShader("DefaultVertexShader.glsl", GL_VERTEX_SHADER);
    loader.LoadBuiltinShader("DefaultFragmentShader.glsl", GL_FRAGMENT_SHADER);
    std::shared_ptr<bc::Shaders> shaders(loader.Build());

    bc::Texture texture("textures/box.jpg");
    GLuint Texture = texture.id;
    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureID = shaders->NewVariable("myTextureSampler");

    // This will identify our vertex buffer
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

    GLuint MatrixID = shaders->NewVariable("MVP");

    glFrontFace(GL_CW);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    float offset = 0;

    do {
        // Use our shader
        shaders->Use();

        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        offset += 0.005;
        glm::mat4 MVP = get_mvp_matrix(screen, cosf(offset));
        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(TextureID, 0);

        Draw();

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
