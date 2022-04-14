#include <iostream>
#include <filesystem>
#include "gl/Renderer.h"
#include "glfw/glfw3.h"
#include "gltf.h"
#include "Engine.h"

std::unique_ptr<Engine> engine{};

void main_loop() {
    engine->Tick();
}

int main(int argc, char* argv[]) {
    std::cout << "Working Directory : " << std::filesystem::current_path() << std::endl;

#ifndef EMSCRIPTEN
    std::string path = "..\\..\\..\\..\\assets\\suzanne.glb";
#else
    std::string path = "./suzanne.glb";
#endif

    engine = std::make_unique<Engine>(Engine::Create(600, 600, "Lawless"));
    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;

    engine->SetRenderer(std::make_shared<gl::Renderer>());
    engine->SetCamera(std::make_shared<Matrix4>(Matrix4::Perspective(110.0f, 1, 100)));

    std::shared_ptr<SceneNode> suzanne = gltf::Load(path);

    std::shared_ptr<SceneNode> node1 = std::make_shared<SceneNode>();
    node1->transform.position = Vector3(4, 0.0, 0.0);
    node1->AddChild(suzanne);

    std::shared_ptr<SceneNode> node2 = std::make_shared<SceneNode>();
    node2->transform.position = Vector3(-4, 0.0, 0.0);
    node2->AddChild(suzanne);

    auto scene = engine->GetScene();
    scene->AddChild(node1);
    scene->AddChild(node2);

    scene->transform.position = Vector3(0.0f, -0.0f, -30.0);

    engine->OnTick([&scene](float deltaMs) {
        scene->transform.rotation.y += 0.001f * deltaMs;
    });

#ifndef EMSCRIPTEN
    while (engine->isRunning()) {
        main_loop();
    }
#else
    emscripten_set_main_loop(main_loop, 0, true);
#endif

    return 0;
}