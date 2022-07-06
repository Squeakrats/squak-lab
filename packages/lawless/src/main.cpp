#include "Player.h"
#include "PlayerCamera.h"
#include "utility.h"
#include <squak/engine/Engine.h>
#include <squak/engine/GLBLoader.h>
#include <squak/engine/rpc.h>
#include <squak/gl/glutils.h>
#include <squak/gltf.h>
#include <squak/websocket/Server.h>

int main(int argc, char* argv[]) {
  std::string path = argv[0];
  std::string assetDir = argv[1];

  Engine& engine = Engine::Init(assetDir);
  engine.GetAssetManager().Register(std::make_shared<GLBLoader>());
  engine.SetRenderer(
    gl::CreateRenderer(engine.GetSize().first, engine.GetSize().second));
  engine.GetScene()->AddChild(
    engine.GetAssetManager().Get<SceneAsset>("map.glb")->scene);
  engine.Spawn<PlayerCamera>();

  std::unique_ptr<websocket::Server> rpcServer{};
  if (argc == 2) {
    rpcServer = std::make_unique<websocket::Server>();
    rpcServer->OnConnection([](websocket::Socket& socket) {
      socket.OnMessage([](std::string m) {
        Engine::engine.Invoke(RPC::Parse(json::Parse(m)));
      });
    });

    rpcServer->Listen("0.0.0.0", 1338);

    spawn(path, assetDir + " client");
  }

  engine.Run();

  return 0;
}