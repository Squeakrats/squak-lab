#include <iostream>
#include <filesystem>
#include "gl/Renderer.h"
#include "Window.h"
#include "gltf.h"

void tick(SceneNode& scene, gl::Renderer& renderer, Matrix4& camera, uint32_t& frameId) {
    renderer.Render(camera, scene);
    frameId++;
}

int main(int argc, char* argv[]) {
    Window window = Window::Create(600, 600, "Lawless");
    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "CWD : " << std::filesystem::current_path() << std::endl;

    gl::Renderer renderer{};
    Matrix4 camera = Matrix4::Identity();

    std::shared_ptr<SceneNode> scene = gltf::Load("..\\..\\..\\..\\assets\\cube.glb");

    uint32_t frameId = 0;
    window.Tick([&scene, &renderer, &camera, &frameId]() {
        tick(*scene, renderer, camera, frameId);
    });

    return 0;
}