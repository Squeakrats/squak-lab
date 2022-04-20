#include "gl/Renderer.h"
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

    engine.GetAssetManager().Register(std::make_shared<GLBLoader>());
    engine.SetRenderer(std::make_shared<gl::Renderer>());

    auto player = engine.Spawn<Player>();
    auto camera = engine.Spawn<PlayerCamera>();
    camera->target = player;
    

    engine.GetScene()->AddChild(engine.GetAsset<SceneAsset>("map.glb")->scene);

    engine.Run();

    return 0;
}