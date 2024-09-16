#include <iostream>
#include "algebra3d.h"
#include "Engine3D.h"
#include <unistd.h>

mesh_t cube({
                    {0, 0, 0, 0, 1, 0, 1, 1, 0},
                    {0, 0, 0, 1, 1, 0, 1, 0, 0},  // Front

                    {0, 0, 1, 0, 1, 1, 1, 1, 1},
                    {0, 0, 1, 1, 1, 1, 1, 0, 1},  // Back

                    {0, 0, 0, 0, 1, 0, 0, 1, 1},
                    {0, 0, 0, 0, 1, 1, 0, 0, 1},  // Left

                    {1, 0, 0, 1, 1, 0, 1, 1, 1},
                    {1, 0, 0, 1, 1, 1, 1, 0, 1},  // Right

                    {0, 0, 0, 0, 0, 1, 1, 0, 1},
                    {0, 0, 0, 1, 0, 1, 1, 0, 0},  // Bottom

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

        std::vector<Color> colors = {
                {1, 0, 0},
                {1, 0, 0},

                {0, 1, 0},
                {0, 1, 0},

                {0, 0, 1},
                {0, 0, 1},

                {1, 1, 0},
                {1, 1, 0},

                {1, 0, 1},
                {1, 0, 1},

                {0, 1, 1},
                {0, 1, 1}
        };

        int c = 0;

        for (const auto &face: cube.faces) {
            triangle_t trnFace = face * (
                    matrix_t::Rotate(xRot, yRot, 0) *
                    matrix_t::Translate(0, 0, 4)
            );

            DrawTriangle(trnFace, colors[c++]);
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