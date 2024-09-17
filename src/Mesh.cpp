//
// Created by Davide Caroselli on 17/09/24.
//

#include "mesh.h"
#include <fstream>
#include <sstream>

mesh_t mesh_t::from_obj(const std::string &filename) {
    std::ifstream file(filename);

    if (!file.is_open())
        throw std::runtime_error("Could not open file");

    std::vector<triangle_t> faces;
    std::vector<vertex_t> vertices;

    std::string line;
    while (std::getline(file, line)) {
        if (line[0] == 'v') {
            std::istringstream iss(line);
            std::string token;
            std::vector<float> coords;
            while (iss >> token) {
                if (token[0] == 'v') continue;
                coords.push_back(std::stof(token));
            }

            if (coords.size() != 3)
                throw std::runtime_error("Invalid vertex format");

            vertex_t vertex{coords[0], coords[1], coords[2]};
            vertices.push_back(vertex);
        } else if (line[0] == 'f') {
            std::istringstream iss(line);
            std::string token;
            std::vector<int> indices;
            while (iss >> token) {
                if (token[0] == 'f') continue;
                std::istringstream index(token);
                std::string idx;
                while (std::getline(index, idx, '/')) {
                    indices.push_back(std::stoi(idx));
                }
            }

            if (indices.size() != 3)
                throw std::runtime_error("Invalid face format");

            triangle_t face{
                vertices[indices[0] - 1],
                vertices[indices[1] - 1],
                vertices[indices[2] - 1]};

            faces.push_back(face);
        }
    }

    return mesh_t(faces);
}
