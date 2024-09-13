#include <iostream>
#include "algebra3d.h"
#include "Engine3D.h"
#include <unistd.h>

mesh_t cube({
                    {0, 0, 0, 0, 1, 0, 1, 1, 0},
                    {0, 0, 0, 1, 1, 0, 1, 0, 0},  // Front

                    {0, 0, 1, 0, 1, 1, 1, 1, 1},
                    {0, 0, 1, 1, 1, 1, 1, 0, 1},  // Back

                    {0, 0, 0, 0, 0, 1, 0, 1, 0},
                    {0, 0, 1, 0, 1, 1, 0, 1, 0},  // Left

                    {1, 0, 0, 1, 0, 1, 1, 1, 0},
                    {1, 0, 1, 1, 1, 1, 1, 1, 0},  // Right

                    {0, 0, 0, 1, 0, 0, 1, 1, 0},
                    {0, 0, 0, 1, 1, 0, 0, 1, 0},  // Bottom

                    {0, 0, 1, 1, 0, 1, 1, 1, 1},
                    {0, 0, 1, 1, 1, 1, 0, 1, 1}   // Top
            });

class GameEngine : public Engine3D {
public:
    explicit GameEngine(const std::string &name) : Engine3D(name, 640, 480, 60) {};

    bool Render(float elapsedTime) override {
        ClearScreen({0.2f, 0.2f, 0.2f});





        return true;
    }
};

int main() {
    GameEngine game("Demo");
    game.SetShowDebugInfo(true);
    game.Start(false);

    return 0;
}