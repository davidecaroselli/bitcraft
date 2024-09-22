//
// Created by Davide Caroselli on 22/09/24.
//

#include "shaders.h"
#include <fstream>
#include <sstream>
#include <filesystem>

namespace bc = bitcraft;
namespace fs = std::filesystem;

bc::shader_t bc::shader_t::builtin(const std::string &name, GLenum type) {
    return {0, type, fs::path("shaders") / name};
}

GLuint bc::LoadShaders(std::vector<bc::shader_t> &shaders) {
    // Compiling shaders -----------------------------------------------------------------------------------------------

    for (auto &shader: shaders) {
        // Reading source code from file
        fs::path path = shader.filepath;
        if (!fs::exists(path))
            throw std::runtime_error("File not found: " + path.string());

        std::string code;

        std::ifstream file(path, std::ios::in);
        if (!file.is_open())
            throw std::runtime_error("Could not open file: " + path.string());

        std::stringstream reader;
        reader << file.rdbuf();
        code = reader.str();
        file.close();

        // Compiling shader
        shader.id = glCreateShader(shader.type);

        char const *ptrCode = code.c_str();
        glShaderSource(shader.id, 1, &ptrCode, nullptr);
        glCompileShader(shader.id);


        GLint compileStatus = GL_FALSE, infoLogLength;
        glGetShaderiv(shader.id, GL_COMPILE_STATUS, &compileStatus);
        glGetShaderiv(shader.id, GL_INFO_LOG_LENGTH, &infoLogLength);

        if (infoLogLength > 0) {
            std::vector<char> cause(infoLogLength + 1);
            glGetShaderInfoLog(shader.id, infoLogLength, nullptr, &cause[0]);
            std::string error = "Error compiling shader " + path.filename().string() + ": "
                                + std::string(cause.begin(), cause.end());

            throw std::runtime_error(error);
        }
    }

    // Linking the program ---------------------------------------------------------------------------------------------
    GLuint program = glCreateProgram();
    for (auto &shader: shaders)
        glAttachShader(program, shader.id);
    glLinkProgram(program);

    GLint linkStatus = GL_FALSE, infoLogLength;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (infoLogLength > 0) {
        std::vector<char> cause(infoLogLength + 1);
        glGetProgramInfoLog(program, infoLogLength, nullptr, &cause[0]);
        std::string error = "Error linking shader program: " + std::string(cause.begin(), cause.end());

        throw std::runtime_error(error);
    }

    // Cleaning up -----------------------------------------------------------------------------------------------------
    for (auto &shader: shaders)
        glDetachShader(program, shader.id);

    for (auto &shader: shaders)
        glDeleteShader(shader.id);

    return program;
}