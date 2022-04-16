#include "gl/Renderer.h"
#include "glfw/glfw3.h"
#include "gltf.h"
#include "Engine.h"
#include "Player.h"

std::unique_ptr<Engine> engine{};

void main_loop() { engine->Tick(); }
void run() {
#ifndef EMSCRIPTEN
    while (engine->isRunning()) { main_loop(); }
#else
    emscripten_set_main_loop(main_loop, 0, true);
#endif
}

#ifdef EMSCRIPTEN
std::string assetDir = "./";
#else
std::string assetDir = "..\\..\\..\\..\\assets\\";
#endif

int main(int argc, char* argv[]) {
    engine = std::make_unique<Engine>(Engine::Create(1000, 1000, "Lawless", assetDir));
    engine->GetAssetManager().Register("glb", std::make_shared<gltf::GLBLoader>());
    engine->RegisterCreator(Player::CREATORENTRY);

    engine->SetRenderer(std::make_shared<gl::Renderer>());
    engine->SetCamera(std::make_shared<Matrix4>(Matrix4::Perspective(110.0f, 1, 100)));

    auto scene = engine->GetScene();
    scene->transform.position = Vector3(0.0f, 0.0f, -30.0);

    auto player = engine->Spawn<Player>();
    player->GetTransform().position = Vector3(0, 0.0, 0.0);
    scene->AddChild(player->GetRoot());

    run();

    return 0;
}