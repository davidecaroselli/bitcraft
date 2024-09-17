//
// Created by Davide Caroselli on 17/09/24.
//

#ifndef BITCRAFT_MESH_H
#define BITCRAFT_MESH_H

#include <vector>
#include "face_t.h"

struct mesh_t {
    std::vector<face_t> faces;

    static mesh_t load_from_obj(const std::string &filename);

    mesh_t() = default;

    explicit mesh_t(const std::vector<face_t> &faces) : faces(faces) {}
};


#endif //BITCRAFT_MESH_H
