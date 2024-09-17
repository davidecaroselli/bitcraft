//
// Created by Davide Caroselli on 17/09/24.
//

#ifndef BITCRAFT_MATRIX_T_H
#define BITCRAFT_MATRIX_T_H


struct matrix_t {
    float v[4][4];

    inline static matrix_t Scale(float v) {
        return Scale(v, v, v);
    }

    static matrix_t Scale(float x, float y, float z);

    static matrix_t Translate(float x, float y, float z);

    static matrix_t Rotate(float ax, float ay, float az);

    float operator()(unsigned int row, unsigned int col) const {
        return v[row][col];
    }

    inline matrix_t operator*(const matrix_t &o) const {
        return {{
                        {
                                v[0][0] * o.v[0][0] + v[0][1] * o.v[1][0] + v[0][2] * o.v[2][0] + v[0][3] * o.v[3][0],
                                v[0][0] * o.v[0][1] + v[0][1] * o.v[1][1] + v[0][2] * o.v[2][1] + v[0][3] * o.v[3][1],
                                v[0][0] * o.v[0][2] + v[0][1] * o.v[1][2] + v[0][2] * o.v[2][2] + v[0][3] * o.v[3][2],
                                v[0][0] * o.v[0][3] + v[0][1] * o.v[1][3] + v[0][2] * o.v[2][3] + v[0][3] * o.v[3][3]
                        },
                        {
                                v[1][0] * o.v[0][0] + v[1][1] * o.v[1][0] + v[1][2] * o.v[2][0] + v[1][3] * o.v[3][0],
                                v[1][0] * o.v[0][1] + v[1][1] * o.v[1][1] + v[1][2] * o.v[2][1] + v[1][3] * o.v[3][1],
                                v[1][0] * o.v[0][2] + v[1][1] * o.v[1][2] + v[1][2] * o.v[2][2] + v[1][3] * o.v[3][2],
                                v[1][0] * o.v[0][3] + v[1][1] * o.v[1][3] + v[1][2] * o.v[2][3] + v[1][3] * o.v[3][3]
                        },
                        {
                                v[2][0] * o.v[0][0] + v[2][1] * o.v[1][0] + v[2][2] * o.v[2][0] + v[2][3] * o.v[3][0],
                                v[2][0] * o.v[0][1] + v[2][1] * o.v[1][1] + v[2][2] * o.v[2][1] + v[2][3] * o.v[3][1],
                                v[2][0] * o.v[0][2] + v[2][1] * o.v[1][2] + v[2][2] * o.v[2][2] + v[2][3] * o.v[3][2],
                                v[2][0] * o.v[0][3] + v[2][1] * o.v[1][3] + v[2][2] * o.v[2][3] + v[2][3] * o.v[3][3]
                        },
                        {
                                v[3][0] * o.v[0][0] + v[3][1] * o.v[1][0] + v[3][2] * o.v[2][0] + v[3][3] * o.v[3][0],
                                v[3][0] * o.v[0][1] + v[3][1] * o.v[1][1] + v[3][2] * o.v[2][1] + v[3][3] * o.v[3][1],
                                v[3][0] * o.v[0][2] + v[3][1] * o.v[1][2] + v[3][2] * o.v[2][2] + v[3][3] * o.v[3][2],
                                v[3][0] * o.v[0][3] + v[3][1] * o.v[1][3] + v[3][2] * o.v[2][3] + v[3][3] * o.v[3][3]
                        }
                }};
    }

};


#endif //BITCRAFT_MATRIX_T_H
