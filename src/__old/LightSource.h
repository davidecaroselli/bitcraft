//
// Created by Davide Caroselli on 21/09/24.
//

#ifndef BITCRAFT_LIGHTSOURCE_H
#define BITCRAFT_LIGHTSOURCE_H

#include "__old/geometry/vertex.h"
#include "__old/geometry/face.h"
#include <algorithm>

class LightSource {
public:
    LightSource() : direction({0, 0, -1}), minBrightness(0), maxBrightness(1) {}

    void SetBrightness(float min, float max) {
        minBrightness = std::clamp(min, 0.0f, 1.0f);
        maxBrightness = std::clamp(max, 0.0f, 1.0f);
    }

    void SetDirection(const vertex_t &d) {
        direction = d / d.length();
    }

    void Apply(face_t &face) const;

private:
    vertex_t direction;
    float minBrightness;
    float maxBrightness;

};


#endif //BITCRAFT_LIGHTSOURCE_H
