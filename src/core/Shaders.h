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
        const GLuint id;
        const GLenum type;
        const std::string name;
    };

    class Shaders {
    public:
        class Loader {
        public:
            Loader();

            void LoadBuiltinShader(const std::string &name, GLenum type);

            void LoadShader(const std::string &filepath, GLenum type);

            [[nodiscard]] Shaders *Build() const;

        private:
            const GLuint program;
            std::vector<shader_t> shaders;
        };

        void Use() const;

        [[nodiscard]] GLuint NewVariable(const std::string &name) const;

    private:
        const GLuint program;

        explicit Shaders(GLuint program) : program(program) {};
    };

}

#endif //BITCRAFT_SHADERS_H
