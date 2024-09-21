//
// Created by Davide Caroselli on 21/09/24.
//

#ifndef BITCRAFT_LIGHTSOURCE_H
#define BITCRAFT_LIGHTSOURCE_H

#include "geometry/vertex.h"

class LightSource {
public:
    LightSource() : direction({0, 0, -1}) {}

    [[nodiscard]] const vertex_t &GetDirection() const {
        return direction;
    }

    void SetDirection(const vertex_t &d) {
        direction = d / d.length();
    }

private:
    vertex_t direction;
};


#endif //BITCRAFT_LIGHTSOURCE_H
