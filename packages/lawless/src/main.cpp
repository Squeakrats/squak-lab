#include "Player.h"
#include "PlayerCamera.h"
#include "utility.h"
#include <squak/engine/Engine.h>
#include <squak/engine/GLBLoader.h>
#include <squak/engine/rpc.h>
#include <squak/gl/glutils.h>
#include <squak/gltf.h>
#include <squak/websocket/Server.h>

void server(std::string path, std::string assetDir) {
  Engine& engine = Engine::engine;

#ifndef EMSCRIPTEN
  websocket::Server server{};
  server.OnConnection([&engine](websocket::Socket& socket) {
    socket.OnMessage(
      [&engine](std::string m) { engine.Invoke(RPC::Parse(json::Parse(m))); });
  });

  server.Listen("0.0.0.0", 1338);
#endif

  spawn(path, assetDir + " client");
  engine.Run();
}

void client() {
  Engine& engine = Engine::engine;
  engine.Run();
}

int main(int argc, char* argv[]) {
  std::string path = argv[0];
  std::string assetDir = argv[1];

  Engine& engine = Engine::Init(assetDir);
  engine.GetAssetManager().Register(std::make_shared<GLBLoader>());
  engine.SetRenderer(
    gl::CreateRenderer(engine.GetSize().first, engine.GetSize().second));
  engine.GetScene()->AddChild(
    engine.GetAssetManager().Get<SceneAsset>("map.glb")->scene);

  auto camera = engine.Spawn<PlayerCamera>();

  if (argc == 2) {
    server(path, assetDir);
  } else {
    client();
  }

  return 0;
}