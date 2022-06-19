#include "Player.h"
#include "PlayerCamera.h"
#include "utility.h"
#include <squak/engine/Engine.h>
#include <squak/engine/GLBLoader.h>
#include <squak/gl/glutils.h>
#include <squak/gltf.h>
#include <squak/websocket/Server.h>

#ifdef EMSCRIPTEN
std::string assetDir = "./";
#else
std::string assetDir = "..\\..\\..\\..\\assets\\";
#endif

std::map<std::string, std::function<void(void)>> methods{};

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

  auto camera = engine.Spawn<PlayerCamera>();
  camera->target = engine.Spawn<Player>();

  websocket::Server server{};
  server.OnConnection([&engine](websocket::Socket& socket) {
    Log("Socket Connected!");
    socket.OnClose([]() { Log("Socket Closed"); });
    socket.OnMessage([&engine](std::string message) {
      json::Object rpc = json::Parse(message);
      std::string id = rpc["id"].get<std::string>();
      std::string method = rpc["method"].get<std::string>();

      auto object = engine.Find(id);
      object->GetRuntimeTypeInfo().methods[method](object.get());
    });
  });

  server.Listen("127.0.0.1", 1338);
  engine.Run();

  return 0;
}