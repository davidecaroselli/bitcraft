#include <iostream>
#include "GameEngine.h"
#include "geometry/mesh.h"
#include <GLUT/glut.h>

class DemoEngine : public GameEngine {
public:
    mesh_t obj;

    explicit DemoEngine(const std::string &name) : GameEngine(name) {
        obj = mesh_t::load_from_obj("../res/ArtisansHub.obj");
    };

    bool OnUpdate(float elapsedTime) override {
        float speed = 20., rotSpeed = 5;
        if (input.IsKeyPressed('w')) camera.MoveForward(speed * elapsedTime);
        if (input.IsKeyPressed('s')) camera.MoveBackward(speed * elapsedTime);
        if (input.IsKeyPressed('a')) camera.Rotate(0, rotSpeed * elapsedTime, 0);
        if (input.IsKeyPressed('d')) camera.Rotate(0, -rotSpeed * elapsedTime, 0);
        if (input.IsKeyPressed('q')) camera.MoveLeft(speed * elapsedTime);
        if (input.IsKeyPressed('e')) camera.MoveRight(speed * elapsedTime);
        if (input.IsKeyPressed('+')) camera.MoveUp(speed * elapsedTime);
        if (input.IsKeyPressed('-')) camera.MoveDown(speed * elapsedTime);

        scene.clear();
        scene.reserve(obj.faces.size());

        for (const auto &face: obj.faces) {
            scene.push_back(face * (
                    matrix_t::Translate(0, 0, 20)
            ));
        }

        return true;
    }
};

int main() {
    DemoEngine game("Demo");
    game.SetClearColor({0.2, 0.2, 0.2});
    game.SetWireframe(true);
    game.ShowDebugInfo();
    game.Start(640, 480, 60, true);

    return 0;
}