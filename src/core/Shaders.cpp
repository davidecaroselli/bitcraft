//
// Created by Davide Caroselli on 22/09/24.
//

#include "Shaders.h"
#include <fstream>
#include <sstream>
#include <filesystem>

using namespace bitcraft;

namespace fs = std::filesystem;

Shaders::Loader::Loader() : program(glCreateProgram()) {}

void Shaders::Loader::LoadBuiltinShader(const std::string &name, GLenum type) {
    LoadShader(fs::path("shaders") / name, type);
}

void Shaders::Loader::LoadShader(const std::string &filepath, GLenum type) {
    // Reading source code from file
    fs::path path = filepath;
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
    GLuint sid = glCreateShader(type);

    char const *ptrCode = code.c_str();
    glShaderSource(sid, 1, &ptrCode, nullptr);
    glCompileShader(sid);

    GLint compileStatus = GL_FALSE, infoLogLength;
    glGetShaderiv(sid, GL_COMPILE_STATUS, &compileStatus);
    glGetShaderiv(sid, GL_INFO_LOG_LENGTH, &infoLogLength);

    if (infoLogLength > 0) {
        std::vector<char> cause(infoLogLength + 1);
        glGetShaderInfoLog(sid, infoLogLength, nullptr, &cause[0]);
        std::string error = "Error compiling shader " + path.filename().string() + ": "
                            + std::string(cause.begin(), cause.end());

        throw std::runtime_error(error);
    }

    shaders.push_back({sid, type, path.filename()});
}

Shaders *Shaders::Loader::Build() const {
    for (const auto &shader: shaders)
        glAttachShader(program, shader.id);

    // Linking the program ---------------------------------------------------------------------------------------------
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

    return new Shaders(program);
}


void Shaders::Use() const {
    glUseProgram(program);
}

GLuint Shaders::NewVariable(const std::string &name) const {
    return glGetUniformLocation(program, name.c_str());
}
