#include <iostream>
#include "GameEngine.h"
#include "geometry/mesh.h"
#include <GLUT/glut.h>

class DemoEngine : public GameEngine {
public:
    mesh_t obj;

    explicit DemoEngine(const std::string &name) : GameEngine(name) {
        obj = mesh_t::load_from_obj("../res/axis.obj");
    };

    float translateX = 0;
    float translateY = 0;
    float translateZ = 10;

    bool OnUpdate(float elapsedTime) override {
        float speed = 20.;
        if (input.IsKeyPressed('w')) translateZ -= speed * elapsedTime;
        if (input.IsKeyPressed('s')) translateZ += speed * elapsedTime;
        if (input.IsKeyPressed('a')) translateX += speed * elapsedTime;
        if (input.IsKeyPressed('d')) translateX -= speed * elapsedTime;
        if (input.IsKeyPressed('q')) translateY += speed * elapsedTime;
        if (input.IsKeyPressed('e')) translateY -= speed * elapsedTime;

        scene.clear();
        scene.reserve(obj.faces.size());

        for (const auto &face: obj.faces) {
            scene.push_back(face * (
                    matrix_t::Translate(translateX, translateY, translateZ)
            ));
        }

        return true;
    }
};

int main() {
    DemoEngine game("Demo");
    game.SetClearColor({0.2, 0.2, 0.2});
    game.SetShowDebugInfo(true);
    game.Start(640, 480, 60, false);

    return 0;
}