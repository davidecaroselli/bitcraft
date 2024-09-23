//
// Created by Davide Caroselli on 23/09/24.
//

#include "Model.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace bitcraft;

glm::vec3 read_vertex(std::istringstream &sLine) {
    std::string token;
    std::vector<float> coords;

    while (sLine >> token)
        coords.push_back(std::stof(token));

    if (coords.size() != 3)
        throw std::runtime_error("Invalid vertex format");

    return {coords[0], coords[1], coords[2]};
}

struct face_i {
    int v, vt, vn;

    static face_i parse(const std::string &token) {
        // format is: v/vt/vn
        int v = 0, vt = 0, vn = 0;

        std::istringstream sToken(token);
        std::string part;
        int i = 0;
        while (std::getline(sToken, part, '/')) {
            switch (i++) {
                case 0:
                    v = std::stoi(part);
                    break;
                case 1:
                    if (!part.empty())
                        vt = std::stoi(part);
                    break;
                case 2:
                    if (!part.empty())
                        vn = std::stoi(part);
                    break;
                default:
                    throw std::runtime_error("Invalid face format: " + token);
            }
        }

        return {v - 1, vt - 1, vn - 1};
    }
};

Model::Model(const std::string &path) {
    std::ifstream file(path);

    if (!file.is_open())
        throw std::runtime_error("Could not open file");

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    auto make_face = [&](const face_i &a, const face_i &b, const face_i &c) {
        glm::vec3 v1 = vertices[a.v];
        glm::vec3 v2 = vertices[b.v];
        glm::vec3 v3 = vertices[c.v];

        glm::vec2 uv1 = a.vt >= 0 ? uvs[a.vt] : glm::vec2(0, 0);
        glm::vec2 uv2 = b.vt >= 0 ? uvs[b.vt] : glm::vec2(0, 0);
        glm::vec2 uv3 = c.vt >= 0 ? uvs[c.vt] : glm::vec2(0, 0);

//        glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));
//        glm::vec3 n1 = a.vn >= 0 ? normals[a.vn] : normal;
//        glm::vec3 n2 = b.vn >= 0 ? normals[b.vn] : normal;
//        glm::vec3 n3 = c.vn >= 0 ? normals[c.vn] : normal;

        this->vertices.push_back(v1);
        this->vertices.push_back(v2);
        this->vertices.push_back(v3);

        this->uvs.push_back(uv1);
        this->uvs.push_back(uv2);
        this->uvs.push_back(uv3);

//        this->normals.push_back(n1);
//        this->normals.push_back(n2);
//        this->normals.push_back(n3);
    };

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream sLine(line);
        std::string token;

        sLine >> token;

        if ("v" == token) {
            vertices.push_back(read_vertex(sLine));
        } else if ("vn" == token) {
            normals.push_back(read_vertex(sLine));
        } else if ("vt" == token) {
            std::string token;
            std::vector<float> coords;

            while (sLine >> token)
                coords.push_back(std::stof(token));

            if (coords.size() != 2)
                throw std::runtime_error("Invalid uv format");

            uvs.push_back({coords[0], coords[1]});
        } else if ("f" == token) {
            std::vector<face_i> indices;
            while (sLine >> token)
                indices.push_back(face_i::parse(token));

            if (indices.size() == 3) {
                make_face(indices[0], indices[1], indices[2]);
            } else if (indices.size() == 4) {
                make_face(indices[0], indices[1], indices[2]);
                make_face(indices[0], indices[2], indices[3]);
            } else {
                throw std::runtime_error("Invalid face format: " + line + " (" + std::to_string(indices.size()) + ")");
            }
        }
    }
}
