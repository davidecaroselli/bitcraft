//
// Created by Davide Caroselli on 22/09/24.
//

#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION

#include <utils/stb_image.h>
#include <iostream>

using namespace bitcraft;

GLuint LoadTexture(const std::string &path) {
    stbi_set_flip_vertically_on_load(true);

    int width, height, n;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &n, 0);
    if (data == nullptr)
        throw std::runtime_error("Failed to load texture: " + path);

    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return id;
}

Texture::Texture(const std::string &path) : id(LoadTexture(path)) {
}
