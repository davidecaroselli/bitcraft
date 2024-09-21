//
// Created by Davide Caroselli on 18/09/24.
//

#ifndef BITCRAFT_COLOR_H
#define BITCRAFT_COLOR_H

struct color_t {
    float r, g, b;

    inline color_t &operator*=(float v) {
        r *= v;
        g *= v;
        b *= v;
        return *this;
    }
};

#endif //BITCRAFT_COLOR_H
