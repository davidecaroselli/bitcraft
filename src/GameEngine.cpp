//
// Created by Davide Caroselli on 12/09/24.
//

#include "GameEngine.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <GLUT/glut.h>
#include <sstream>

// Draw functions ---------------------------------------------------

void ClearScreen(const color_t &color) {
    glClearColor(color.r, color.g, color.b, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DrawTriangle(const face_t &triangle, const color_t &color) {
    glBegin(GL_LINE_LOOP);
    glColor3f(color.r, color.g, color.b);
    for (const auto &v: triangle.vs)
        glVertex2f(v.x, v.y);
    glEnd();
}

void FillTriangle(const face_t &triangle, const color_t &color) {
    glBegin(GL_TRIANGLES);
    glColor3f(color.r, color.g, color.b);
    for (const auto &v: triangle.vs)
        glVertex2f(v.x, v.y);
    glEnd();
}

// Utility classes --------------------------------------------------

class ScreenCallback : public IScreenCallback {
public:
    explicit ScreenCallback(GameEngine *engine) : engine(engine) {}

    void OnRender(float elapsedTime) override {
        engine->OnRender(elapsedTime);
    }

    void OnResize() override {
        engine->ComputeProjectionMatrix();
    }

private:
    GameEngine *engine;
};

void DebugInfoView::OnRender(float elapsedTime) {
    renderCount++;
    tElapsedTime += elapsedTime;

    if (tElapsedTime >= updateSecs) {
        fps = (float) renderCount / tElapsedTime;
        renderCount = 0;
        tElapsedTime = 0;
    }
}

void DebugInfoView::DrawToScreen(Screen &screen) const {
    std::stringstream stream;
    stream << "FPS: " << std::fixed << std::setprecision(1) << fps;
    std::string fpsLog = stream.str();

    float glScaleX = 2.f / (float) screen.GetWidth();
    float glScaleY = 2.f / (float) screen.GetHeight();
    float labelWidth = 8 * (float) fpsLog.size();

    glColor3f(0, 0, 0);
    glRectf(-1, -1, (8 + labelWidth + 8) * glScaleX - 1, 20 * glScaleY - 1);

    glColor3f(1, 1, 1);
    glRasterPos2f(8 * glScaleX - 1, 6 * glScaleY - 1);
    for (char c: fpsLog)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
}

// Game engine ------------------------------------------------------

GameEngine::GameEngine(std::string name) : name(std::move(name)), screen(new ScreenCallback(this)) {
}

void GameEngine::Start(unsigned int width, unsigned int height, unsigned int fps, bool fullscreen) {
    screen.Init(name, width, height, fps);
    screen.SetFullscreen(fullscreen);

    input.Init();

    ComputeProjectionMatrix();
    glutMainLoop();
}

void GameEngine::ComputeProjectionMatrix() {
    auto a = screen.GetAspectRatio();
    auto f = (float) (1. / tan(fov * 0.5 * M_PI / 180.));
    auto q = zFar / (zFar - zNear);

    this->prjMatrix = {
            a * f, 0, 0, 0,
            0, f, 0, 0,
            0, 0, q, 1,
            0, 0, -zNear * q, 0
    };
}

void GameEngine::OnRender(float elapsedTime) {
    debugView.OnRender(elapsedTime);

    std::vector<face_t> faces;

    ClearScreen(clearColor);
    OnUpdate(elapsedTime);

    faces = CollectVisibleFaces(scene);
    faces = ApplyCamera(faces);
    faces = ApplyDepthClipping(faces);
    faces = ApplyLighting(faces);
    faces = ApplyProjection(faces);
    faces = SortByDepth(faces);
    faces = ApplyScreenClipping(faces);

    Rasterize(faces);

    // Draw debug info if enabled
    if (showDebugInfo)
        debugView.DrawToScreen(screen);
}

std::vector<face_t> GameEngine::CollectVisibleFaces(std::vector<face_t> &scene) const {
    const vertex_t &cameraPos = camera.GetPosition();

    std::vector<face_t> faces;
    faces.reserve(scene.size());

    for (auto &face: scene) {
        vertex_t normal = face.normal();

        if (normal * (face[0] - cameraPos) < 0)
            faces.push_back(face);
    }

    return faces;
}

std::vector<face_t> &GameEngine::ApplyCamera(std::vector<face_t> &scene) const {
    matrix_t viewMatrix = camera.GetViewMatrix();
    for (auto &face: scene)
        face *= viewMatrix;
    return scene;
}

std::vector<face_t> GameEngine::ApplyDepthClipping(std::vector<face_t> &scene, bool debugColors) const {
    std::vector<face_t> clipFaces;
    clipFaces.reserve(scene.size());

    for (const auto &face: scene) {
        std::vector<face_t> clipped;
        Clip(zNearPlane, face, clipped, debugColors);
        clipFaces.insert(clipFaces.end(), clipped.begin(), clipped.end());
    }

    return clipFaces;
}

std::vector<face_t> &GameEngine::ApplyLighting(std::vector<face_t> &scene) const {
    const vertex_t &lightDirection = light.GetDirection();

    for (auto &face: scene) {
        vertex_t normal = face.normal();
        float brightness = fmax(.5f, normal * lightDirection);
        face.color *= brightness;
    }

    return scene;
}

std::vector<face_t> &GameEngine::ApplyProjection(std::vector<face_t> &scene) const {
    for (auto &face: scene)
        face *= prjMatrix;
    return scene;
}

std::vector<face_t> &GameEngine::SortByDepth(std::vector<face_t> &scene) const {
    std::sort(scene.begin(), scene.end(), [](const face_t &a, const face_t &b) {
        float az = (a[0].z + a[1].z + a[2].z) / 3.f;
        float bz = (b[0].z + b[1].z + b[2].z) / 3.f;
        return az > bz;
    });

    return scene;
}

std::vector<face_t> GameEngine::ApplyScreenClipping(std::vector<face_t> &scene, bool debugColors) const {
    std::vector<face_t> clipFaces;
    clipFaces.reserve(scene.size());

    for (const auto &face: scene) {
        std::vector<face_t> toClip, clipped;
        toClip.push_back(face);

        for (const plane_t &plane: screen.bounds) {
            for (const auto &current: toClip)
                Clip(plane, current, clipped, debugColors);

            toClip = clipped;
        }

        clipFaces.insert(clipFaces.end(), clipped.begin(), clipped.end());
    }

    return clipFaces;
}

void GameEngine::Rasterize(std::vector<face_t> &scene) const {
    for (const auto &face: scene) {
        FillTriangle(face, face.color);
        if (wireframe)
            DrawTriangle(face, {1, 1, 1});
    }
}

void GameEngine::Clip(const plane_t &plane, const face_t &face, std::vector<face_t> &output, bool debugColors) {
    float d0 = plane.Distance(face[0]);
    float d1 = plane.Distance(face[1]);
    float d2 = plane.Distance(face[2]);

    std::vector<const vertex_t *> inside, outside;
    if (d0 >= 0) inside.push_back(&face[0]); else outside.push_back(&face[0]);
    if (d1 >= 0) inside.push_back(&face[1]); else outside.push_back(&face[1]);
    if (d2 >= 0) inside.push_back(&face[2]); else outside.push_back(&face[2]);

    switch (inside.size()) {
        case 1: {
            vertex_t p0 = *inside[0];
            vertex_t p1 = plane.Intersection(p0, *outside[0]);
            vertex_t p2 = plane.Intersection(p0, *outside[1]);

            output.emplace_back(p0, p1, p2, debugColors ? color_t({1, 0, 0}) : face.color);

            break;
        }
        case 2: {
            vertex_t p0 = *inside[0];
            vertex_t p1 = *inside[1];

            vertex_t p2a = plane.Intersection(p0, *outside[0]);
            vertex_t p2b = plane.Intersection(p1, *outside[0]);

            output.emplace_back(p0, p1, p2a, debugColors ? color_t({0, 1, 0}) : face.color);
            output.emplace_back(p1, p2a, p2b, debugColors ? color_t({0, 0, 1}) : face.color);

            break;
        }
        case 3: {
            output.push_back(face);
            break;
        }
    }

}