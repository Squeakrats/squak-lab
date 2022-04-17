#include "gl/Renderer.h"
#include "glfw/glfw3.h"
#include "gltf.h"
#include "Engine.h"
#include "Player.h"

Engine engine{};

void main_loop() { engine.Tick(); }
int run() {
#ifndef EMSCRIPTEN
    while (engine.isRunning()) { main_loop(); }
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

int main(int argc, char* argv[]) {
    engine.GetAssetManager()
        .SetBasePath(assetDir)
        .Register(std::make_shared<gltf::GLBLoader>());

    engine.InitWindow(1000, 1000, "lawless");
    engine.RegisterClass<Player>();
    engine.SetRenderer(std::make_shared<gl::Renderer>());

    engine.RegisterAxis("horizontal", { { { GLFW_KEY_A, -1.0f}, { GLFW_KEY_D,  1.0f } } });
    engine.RegisterAxis("vertical", { { { GLFW_KEY_W, 1.0f}, { GLFW_KEY_S,  -1.0f } } });

    auto camera = std::make_shared<CameraNode>(Matrix4::Perspective(110.0f, 1, 100));
    camera->transform.position.z = 70.0f;
    engine.SetCamera(camera);

    engine.AddChild(engine.Spawn<Player>());

    return run();
}