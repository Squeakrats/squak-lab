#include <squak/engine/Engine.h>
#include <squak/engine/GLBLoader.h>
#include "Player.h"
#include "PlayerCamera.h"
#include <squak/gl/glutils.h>
#include <squak/gltf.h>

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

  std::shared_ptr<SceneNode> light0 =
    std::make_shared<SceneNode>(Transform(Vector3(0.0, 0.0, 10.0)));
  light0->light = std::make_shared<Light>(Light{ Vector3(0.3f, 0.3f, 0.3f) });
  engine.GetScene()->AddChild(light0);

  std::shared_ptr<SceneNode> light1 =
    std::make_shared<SceneNode>(Transform(Vector3(0.0, 10.0, 0.0)));
  light1->light = std::make_shared<Light>(Light{ Vector3(0.3f, 0.3f, 0.3f) });
  engine.GetScene()->AddChild(light1);

  auto map = engine.GetAssetManager().Get<SceneAsset>("map.glb")->scene;
  map->transform.position.y = -2;
  engine.GetScene()->AddChild(map);

  auto player = engine.Spawn<Player>();
  auto camera = engine.Spawn<PlayerCamera>();
  camera->target = player;

  engine.Run();

  return 0;
}