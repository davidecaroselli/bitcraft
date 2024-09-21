//
// Created by Davide Caroselli on 21/09/24.
//

#include "LightSource.h"

void LightSource::Apply(face_t &face) const {
    vertex_t normal = face.normal();
    float brightness = normal * direction;
    if (brightness == 0) {
        face.color = {1, 0, 0};
    } else {
        face.color *= minBrightness + brightness * (maxBrightness - minBrightness);
    }
}
