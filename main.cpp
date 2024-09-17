#include <iostream>
#include "algebra3d.h"
#include "Engine3D.h"
#include <mesh.h>

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
    vertex_t camera = {0, 0, 0};
    vertex_t light = {0, 0, -1};
    mesh_t obj;

    explicit GameEngine(const std::string &name) : Engine3D(name) {
        obj = mesh_t::from_obj("../res/teapot.obj");
    };

    float xRot = 0;
    float yRot = 0;

    bool Render(float elapsedTime) override {
        xRot += 2.f * elapsedTime;
        yRot += 1.4f * elapsedTime;

        if (xRot > M_PI * 2) xRot -= M_PI * 2;
        if (yRot > M_PI * 2) yRot -= M_PI * 2;

        ClearScreen({0.2f, 0.2f, 0.2f});

        std::vector<triangle_t> scene;
        scene.reserve(obj.faces.size());

        for (const auto &face: obj.faces) {
            scene.push_back(face * (
                    matrix_t::Translate(-.5, -.5, -.5) *
                    matrix_t::Rotate(xRot, yRot, 0) *
                    matrix_t::Translate(0, 0, 10)
            ));
        }

        std::sort(scene.begin(), scene.end(), [](const triangle_t &a, const triangle_t &b) {
            float az = (a.v[0].z + a.v[1].z + a.v[2].z) / 3.f;
            float bz = (b.v[0].z + b.v[1].z + b.v[2].z) / 3.f;
            return az > bz;
        });

        for (const auto &face: scene) {
            vertex_t normal = face.normal();

            if (normal * (face.v[0] - camera) < 0) {
                float color = normal * light;
                FillTriangle(face, {color, color, color});
            }
        }

        return true;
    }
};

int main() {
    GameEngine game("Demo");
    game.SetShowDebugInfo(true);
    game.Start(640, 480, 60, true);

    return 0;
}