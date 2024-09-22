//
// Created by Davide Caroselli on 22/09/24.
//

#ifndef BITCRAFT_TEXTURE_H
#define BITCRAFT_TEXTURE_H

#include <GL/glew.h>
#include <string>

namespace bitcraft {

    class Texture {
    public:
        const GLuint id;

        explicit Texture(const std::string &path);
    };

}

#endif //BITCRAFT_TEXTURE_H
