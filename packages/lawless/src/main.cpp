#include <iostream>
#include <filesystem>
#include "gl/Renderer.h"
#include "glfw/glfw3.h"
#include "gltf.h"
#include "Engine.h"

std::unique_ptr<Engine> engine{};

void main_loop() { engine->Tick(); }

#ifdef EMSCRIPTEN
std::string assetDir = "./";
#else
std::string assetDir = "..\\..\\..\\..\\assets\\";
#endif

std::shared_ptr<SceneNode> CreateSuzanne(Vector3 position) {
    gltf::GLBLoader::Asset& suzanne = static_cast<gltf::GLBLoader::Asset&>(*(engine->GetAssetManager().Get(assetDir + "suzanne.glb")));

    std::shared_ptr<SceneNode> node = std::make_shared<SceneNode>();
    node->transform.position = position;
    node->AddChild(suzanne.scene);

    return node;
}

int main(int argc, char* argv[]) {
    engine = std::make_unique<Engine>(Engine::Create(600, 600, "Lawless"));
    std::cout << "Working Directory : " << std::filesystem::current_path() << std::endl;
    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;

    AssetManager& assetManager = engine->GetAssetManager();
    assetManager.Register("glb", std::make_shared<gltf::GLBLoader>());
    assetManager.Register(assetDir +  "suzanne.glb");

    engine->SetRenderer(std::make_shared<gl::Renderer>());
    engine->SetCamera(std::make_shared<Matrix4>(Matrix4::Perspective(110.0f, 1, 100)));

    auto scene = engine->GetScene();
    scene->transform.position = Vector3(0.0f, -0.0f, -30.0);
    scene->AddChild(CreateSuzanne(Vector3(4, 0.0, 0.0)));
    scene->AddChild(CreateSuzanne(Vector3(-4, 0.0, 0.0)));

    engine->OnTick([&scene](float deltaMs) {
        scene->transform.rotation.y += 0.001f * deltaMs;
    });

#ifndef EMSCRIPTEN
    while (engine->isRunning()) { main_loop(); }
#else
    emscripten_set_main_loop(main_loop, 0, true);
#endif

    return 0;
}