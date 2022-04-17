#include "gl/Renderer.h"
#include "glfw/glfw3.h"
#include "gltf.h"
#include "Engine.h"
#include "Player.h"

#ifdef EMSCRIPTEN
std::string assetDir = "./";
#else
std::string assetDir = "..\\..\\..\\..\\assets\\";
#endif

int main(int argc, char* argv[]) {
    Engine& engine = Engine::Init(1000, 1000, "lawless");

    engine.GetAssetManager()
        .SetBasePath(assetDir)
        .Register(std::make_shared<gltf::GLBLoader>());

    engine.SetRenderer(std::make_shared<gl::Renderer>());

    engine.RegisterAxis("horizontal", { { { GLFW_KEY_A, -1.0f}, { GLFW_KEY_D,  1.0f } } });
    engine.RegisterAxis("vertical", { { { GLFW_KEY_W, 1.0f}, { GLFW_KEY_S,  -1.0f } } });

    auto camera = std::make_shared<CameraNode>(Matrix4::Perspective(110.0f, 1, 100));
    camera->transform.position.z = 70.0f;
    engine.SetCamera(camera);

    engine.AddChild(engine.Spawn<Player>());

    engine.Run();

    return 0;
}