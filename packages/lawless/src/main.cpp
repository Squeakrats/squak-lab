#include <iostream>
#include "gl/Renderer.h"
#include "Window.h"
#include "SceneNode.h"

void tick(SceneNode& scene, gl::Renderer& renderer, Matrix4& camera, uint32_t& frameId) {
    scene.transform.SetPosition(std::sin(static_cast<float>(frameId) / 9000) * 300.0f, 0.0f, 0.f);
    renderer.Render(camera, scene);
    frameId++;
}

int main(int argc, char* argv[]) {
    Window window = Window::Create(600, 600, "Lawless");
    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;

    gl::Renderer renderer{};
    Matrix4 camera = Matrix4::Orthographic(-300, 300, -300, 300, -300, 300);

    SceneNode scene{ Matrix4::Identity() };
    scene.geometry = std::make_shared<Geometry>(Geometry::CreatePlane(200, 200));

    uint32_t frameId = 0;
    window.Tick([&scene, &renderer, &camera, &frameId]() {
        tick(scene, renderer, camera, frameId);
    });

    return 0;
}