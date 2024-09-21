#include <iostream>
#include "Engine3D.h"
#include "geometry/mesh.h"
#include <GLUT/glut.h>

class GameEngine : public Engine3D {
public:
    vertex_t light{};
    mesh_t obj;

    explicit GameEngine(const std::string &name) : Engine3D(name) {
        obj = mesh_t::load_from_obj("../res/axis.obj");

        light = {1, 1, -1};
        light = light / light.length();
    };

    float translateX = 0;
    float translateY = 0;
    float translateZ = 10;

    bool OnUpdate(float elapsedTime) override {
        float speed = 80.;
        if (input.IsKeyPressed('w')) translateZ -= speed * elapsedTime;
        if (input.IsKeyPressed('s')) translateZ += speed * elapsedTime;
        if (input.IsKeyPressed('a')) translateX += speed * elapsedTime;
        if (input.IsKeyPressed('d')) translateX -= speed * elapsedTime;
        if (input.IsKeyPressed('q')) translateY += speed * elapsedTime;
        if (input.IsKeyPressed('e')) translateY -= speed * elapsedTime;

        ClearScreen({0.2f, 0.2f, 0.2f});

        std::vector<face_t> scene;
        scene.reserve(obj.faces.size());

        for (const auto &face: obj.faces) {
            scene.push_back(face * (
                    matrix_t::Translate(translateX, translateY, translateZ)
            ));
        }

        std::vector<face_t> facesToRaster;
        facesToRaster.reserve(scene.size());

        for (auto &face: scene) {
            vertex_t normal = face.normal();

            if (normal * (face[0] - camera.GetPosition()) < 0) {
                float color = normal * light;
                face.color = {color, color, color};

                facesToRaster.push_back(face * prjMatrix);
            }
        }

        // Rasterize

        std::sort(facesToRaster.begin(), facesToRaster.end(), [](const face_t &a, const face_t &b) {
            float az = (a[0].z + a[1].z + a[2].z) / 3.f;
            float bz = (b[0].z + b[1].z + b[2].z) / 3.f;
            return az > bz;
        });

        for (const auto &face: facesToRaster) {
            FillTriangle(face, face.color);
            DrawTriangle(face, {1, 1, 1});
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