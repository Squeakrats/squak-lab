#include "Player.h"
#include "PlayerCamera.h"
#include "utility.h"
#include <squak/engine/Engine.h>
#include <squak/engine/GLBLoader.h>
#include <squak/engine/rpc.h>
#include <squak/gl/glutils.h>
#include <squak/gltf.h>
#include <squak/websocket/Server.h>

#ifdef EMSCRIPTEN
std::string assetDir = "./";
#else
std::string assetDir = "..\\..\\..\\..\\assets\\";
#endif

RPC parseRPC(std::string wire) {
  json::Object source = json::Parse(wire);

  RPC rpc{};
  rpc.id = source["id"].get<std::string>();
  rpc.method = source["method"].get<std::string>();

  if (source.has("arguments")) {
    for (auto& argument : source["arguments"].get<json::Array>()) {
      if (std::holds_alternative<double>(argument.value)) {
        rpc.arguments.push_back(std::get<double>(argument.value));
      } else if (std::holds_alternative<std::string>(argument.value)) {
        rpc.arguments.push_back(std::get<std::string>(argument.value));
      } else if (std::holds_alternative<bool>(argument.value)) {
        rpc.arguments.push_back(std::get<bool>(argument.value));
      } else {
        Panic("Unhandled value")
      }
    }
  }

  return rpc;
}

int main(int argc, char* argv[]) {
  Engine& engine = Engine::Init(assetDir);
  auto size = engine.GetSize();

  engine.GetAssetManager().Register(std::make_shared<GLBLoader>());
  engine.SetRenderer(gl::CreateRenderer(size.first, size.second));

  auto light0 = std::make_shared<SceneNode>(Transform(Vector3(0.0, 0.0, 10.0)));
  light0->light = std::make_shared<Light>(Vector3(0.3f, 0.3f, 0.3f));
  engine.GetScene()->AddChild(light0);

  auto light1 = std::make_shared<SceneNode>(Transform(Vector3(0.0, 10.0, 0.0)));
  light1->light = std::make_shared<Light>(Vector3(0.3f, 0.3f, 0.3f));
  engine.GetScene()->AddChild(light1);

  auto map = engine.GetAssetManager().Get<SceneAsset>("map.glb")->scene;
  map->transform.position.y = -2;
  engine.GetScene()->AddChild(map);

  auto camera = engine.Spawn<PlayerCamera>();
  camera->target = engine.Spawn<Player>();

#ifndef EMSCRIPTEN
  websocket::Server server{};
  server.OnConnection([&engine](websocket::Socket& socket) {
    Log("Socket Connected!");
    socket.OnClose([] { Log("Socket Closed"); });
    socket.OnMessage([&engine](std::string m) { engine.Invoke(parseRPC(m)); });
  });

  server.Listen("0.0.0.0", 1338);
#endif
  engine.Run();

  return 0;
}