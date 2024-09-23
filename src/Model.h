//
// Created by Davide Caroselli on 23/09/24.
//

#ifndef BITCRAFT_MODEL_H
#define BITCRAFT_MODEL_H

#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace bitcraft {

    class Model {
    public:
        explicit Model(const std::string &path);

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
    };

}


#endif //BITCRAFT_MODEL_H
