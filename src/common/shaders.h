//
// Created by Davide Caroselli on 22/09/24.
//

#ifndef BITCRAFT_SHADERS_H
#define BITCRAFT_SHADERS_H

#include <GL/glew.h>
#include <string>
#include <vector>

namespace bitcraft {

    struct shader_t {
        GLuint id;
        const GLenum type;
        const std::string filepath;

        static shader_t builtin(const std::string &name, GLenum type);
    };

    GLuint LoadShaders(std::vector<shader_t> &shaders);

}

#endif //BITCRAFT_SHADERS_H
