#include <iostream>
#include "Engine3D.h"
#include "geometry/mesh.h"

class GameEngine : public Engine3D {
public:
    vertex_t camera{};
    vertex_t light{};
    mesh_t obj;

    explicit GameEngine(const std::string &name) : Engine3D(name) {
        obj = mesh_t::load_from_obj("../res/teapot.obj");

        camera = {0, 0, 0};

        light = {0, 0, -1};
        light = light / light.length();
    };

    float xRot = 0;
    float yRot = 0;

    bool Render(float elapsedTime) override {
        xRot += 2.f * elapsedTime;
        yRot += 1.4f * elapsedTime;

        if (xRot > M_PI * 2) xRot -= M_PI * 2;
        if (yRot > M_PI * 2) yRot -= M_PI * 2;

        ClearScreen({0.2f, 0.2f, 0.2f});

        std::vector<face_t> scene;
        scene.reserve(obj.faces.size());

        for (const auto &face: obj.faces) {
            scene.push_back(face * (
                    matrix_t::Translate(-.5, -.5, -.5) *
                    matrix_t::Rotate(xRot, yRot, 0) *
                    matrix_t::Translate(0, 0, 10)
            ));
        }

        std::sort(scene.begin(), scene.end(), [](const face_t &a, const face_t &b) {
            float az = (a[0].z + a[1].z + a[2].z) / 3.f;
            float bz = (b[0].z + b[1].z + b[2].z) / 3.f;
            return az > bz;
        });

        for (const auto &face: scene) {
            vertex_t normal = face.normal();

            if (normal * (face[0] - camera) < 0) {
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
    game.Start(640, 480, 60, false);

    return 0;
}