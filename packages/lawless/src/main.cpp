#include "gl/glutils.h"
#include "glfw/glfw3.h"
#include "gltf.h"
#include "Engine.h"
#include "Player.h"
#include "PlayerCamera.h"
#include "GLBLoader.h"

#ifdef EMSCRIPTEN
std::string assetDir = "./";
#else
std::string assetDir = "..\\..\\..\\..\\assets\\";
#endif

int main(int argc, char* argv[]) {
    Engine& engine = Engine::Init(assetDir);
    auto size = engine.GetSize();

    engine.GetAssetManager().Register(std::make_shared<GLBLoader>());
    engine.SetRenderer(gl::CreateRenderer(size.first, size.second));

    auto player = engine.Spawn<Player>();
    auto camera = engine.Spawn<PlayerCamera>();
    camera->target = player;

    engine.Run();

    return 0;
}