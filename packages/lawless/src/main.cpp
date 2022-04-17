#include "gl/Renderer.h"
#include "glfw/glfw3.h"
#include "gltf.h"
#include "Engine.h"
#include "Player.h"

std::unique_ptr<Engine> engine{};

void main_loop() { engine->Tick(); }
int run() {
#ifndef EMSCRIPTEN
    while (engine->isRunning()) { main_loop(); }
#else
    emscripten_set_main_loop(main_loop, 0, true);
#endif
    
    return 0;
}

#ifdef EMSCRIPTEN
std::string assetDir = "./";
#else
std::string assetDir = "..\\..\\..\\..\\assets\\";
#endif

void createMap() {
    auto asset = engine->GetAsset<SceneAsset>("cube.glb")->scene;
    for (float x = -10.0f; x < 10.0f; x++) {
        for (float y = -10.0f; y < 10.0f; y++) {
            auto cube = std::make_shared<SceneNode>(Transform(
                Vector3(x * 5.0f, y * 2.0f, -70.0)
            ));

            cube->AddChild(asset);
            engine->GetScene()->AddChild(cube);
        }
    }
}

int main(int argc, char* argv[]) {
    engine = std::make_unique<Engine>(Engine::Create(1000, 1000, "Lawless", assetDir));
    engine->GetAssetManager().Register("glb", std::make_shared<gltf::GLBLoader>());
    engine->RegisterCreator(Player::CREATORENTRY);
    engine->SetRenderer(std::make_shared<gl::Renderer>());

    engine->RegisterAxis("horizontal", { {
        { GLFW_KEY_A, -1.0f},
        { GLFW_KEY_D,  1.0f}
    } });

    auto camera = std::make_shared<CameraNode>(Matrix4::Perspective(110.0f, 1, 100));
    engine->SetCamera(camera);

    createMap();

    auto player = engine->Spawn<Player>(Transform(Vector3(0, 0.0, -70.0)));
    engine->AddChild(player);

    engine->OnTick([&camera, &player](float deltaMs) {
        Vector3 difference = .1f * (player->GetTransform().position - camera->transform.position);
        difference.z = 0.0f;

        camera->transform.position += difference;
    });

    return run();
}