#include <iostream>
#include "algebra3d.h"
#include "Engine3D.h"
#include <unistd.h>

mesh_t cube({
                    {0, 0, 0, 0, 1, 0, 1, 1, 0},
                    {0, 0, 0, 1, 1, 0, 1, 0, 0},  // Front

                    {0, 0, 1, 1, 1, 1, 0, 1, 1},
                    {0, 0, 1, 1, 0, 1, 1, 1, 1},  // Back

                    {0, 0, 0, 0, 1, 1, 0, 1, 0},
                    {0, 0, 0, 0, 0, 1, 0, 1, 1},  // Left

                    {1, 0, 0, 1, 1, 0, 1, 1, 1},
                    {1, 0, 0, 1, 1, 1, 1, 0, 1},  // Right

                    {0, 0, 0, 1, 0, 1, 0, 0, 1},
                    {0, 0, 0, 1, 0, 0, 1, 0, 1},  // Bottom

                    {0, 1, 0, 0, 1, 1, 1, 1, 1},
                    {0, 1, 0, 1, 1, 1, 1, 1, 0}   // Top
            });

class GameEngine : public Engine3D {
public:
    explicit GameEngine(const std::string &name) : Engine3D(name) {};

    float xRot = 0;
    float yRot = 0;

    bool Render(float elapsedTime) override {
        xRot += 2.f * elapsedTime;
        yRot += 1.4f * elapsedTime;

        if (xRot > M_PI * 2) xRot -= M_PI * 2;
        if (yRot > M_PI * 2) yRot -= M_PI * 2;

        ClearScreen({0.2f, 0.2f, 0.2f});

        for (const auto &face: cube.faces) {
            triangle_t trnFace = face * (
                    matrix_t::Translate(-.5, -.5, -.5) *
                    matrix_t::Rotate(xRot, yRot, 0) *
                    matrix_t::Translate(0, 0, 4)

            );
            vertex_t normal = trnFace.normal();
            vertex_t camera = {0, 0, 0};
            vertex_t light = {0, 0, -1};

            float color = normal * light;

            if (normal * (trnFace.v[0] - camera) < 0)
                FillTriangle(trnFace, {color, color, color});
        }

        return true;
    }
};

int main() {
    GameEngine game("Demo");
    game.SetShowDebugInfo(true);
    game.Start(640, 480, 60, false);

    return 0;
}