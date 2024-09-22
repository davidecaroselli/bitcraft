//
// Created by Davide Caroselli on 17/09/24.
//

#include "mesh.h"
#include <fstream>
#include <sstream>
#include <algorithm>

vertex_t read_vertex(std::istringstream &sLine) {
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

mesh_t mesh_t::load_from_obj(const std::string &filename) {
    std::ifstream file(filename);

    if (!file.is_open())
        throw std::runtime_error("Could not open file");

    std::vector<face_t> faces;
    std::vector<vertex_t> vertices;
    std::vector<vertex_t> normals;

    auto make_face = [&](const face_i &a, const face_i &b, const face_i &c) {
        vertex_t v1 = vertices[a.v];
        vertex_t v2 = vertices[b.v];
        vertex_t v3 = vertices[c.v];

        vertex_t n = {0, 0, 0};

        int vn = std::max(a.vn, std::max(b.vn, c.vn));
        if (vn > 0) n = normals[vn];

        return face_t(v1, v2, v3, {1, 1, 1}, n);
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
        } else if ("f" == token) {
            std::vector<face_i> indices;
            while (sLine >> token)
                indices.push_back(face_i::parse(token));

            if (indices.size() == 3) {
                faces.push_back(make_face(indices[0], indices[1], indices[2]));
            } else if (indices.size() == 4) {
                faces.push_back(make_face(indices[0], indices[1], indices[2]));
                faces.push_back(make_face(indices[0], indices[2], indices[3]));
            } else {
                throw std::runtime_error("Invalid face format: " + line + " (" + std::to_string(indices.size()) + ")");
            }
        }
    }

    return mesh_t(faces);
}
