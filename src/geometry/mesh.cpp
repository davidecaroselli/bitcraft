//
// Created by Davide Caroselli on 17/09/24.
//

#include "mesh.h"
#include <fstream>
#include <sstream>

mesh_t mesh_t::load_from_obj(const std::string &filename) {
    std::ifstream file(filename);

    if (!file.is_open())
        throw std::runtime_error("Could not open file");

    std::vector<face_t> faces;
    std::vector<vertex_t> vertices;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream sLine(line);
        std::string token;

        sLine >> token;

        if ("v" == token) {
            std::vector<float> coords;
            while (sLine >> token)
                coords.push_back(std::stof(token));

            if (coords.size() != 3)
                throw std::runtime_error("Invalid vertex format: " + line);

            vertices.emplace_back(coords[0], coords[1], coords[2]);
        } else if ("f" == token) {
            std::vector<int> indices;
            while (sLine >> token) {
                auto slashPos = token.find('/');
                if (slashPos != std::string::npos)
                    token = token.substr(0, slashPos);

                indices.push_back(std::stoi(token));
            }

            if (indices.size() == 3) {
                faces.emplace_back(vertices[indices[0] - 1],
                                   vertices[indices[1] - 1],
                                   vertices[indices[2] - 1]);
            } else if (indices.size() == 4) {
                faces.emplace_back(vertices[indices[0] - 1],
                                   vertices[indices[1] - 1],
                                   vertices[indices[2] - 1]);
                faces.emplace_back(vertices[indices[0] - 1],
                                   vertices[indices[2] - 1],
                                   vertices[indices[3] - 1]);
            } else {
                throw std::runtime_error("Invalid face format: " + line + " (" + std::to_string(indices.size()) + ")");
            }
        }
    }

    return mesh_t(faces);
}
