#include "gl/Renderer.h"
#include "glfw/glfw3.h"
#include "gltf.h"
#include "Engine.h"
#include "SceneAsset.h"

std::unique_ptr<Engine> engine{};

void main_loop() { engine->Tick(); }
void run() {
#ifndef EMSCRIPTEN
    while (engine->isRunning()) { main_loop(); }
#else
    emscripten_set_main_loop(main_loop, 0, true);
#endif
}

#ifdef EMSCRIPTEN
std::string assetDir = "./";
#else
std::string assetDir = "..\\..\\..\\..\\assets\\";
#endif

class Suzzane : public Actor {
public:
    using Super = Actor;

    const static float DEFAULT_SPEED;

    Suzzane(std::string id) : Actor(id) {
        this->root = std::make_shared<SceneNode>();
        this->root->AddChild(engine->GetAsset<SceneAsset>("suzanne.glb")->scene);
    }

    virtual void Tick(float deltaMs) override {
        Super::Tick(deltaMs);
        this->GetTransform().rotation.y += .001f * deltaMs;

        Vector3 movement(0, 0, 0);
        if (engine->GetKey(GLFW_KEY_W)) {
            movement.z -= .01f * deltaMs;
        }
        else if (engine->GetKey(GLFW_KEY_S)) {
            movement.z += .01f * deltaMs;
        }

        this->GetTransform().position += DEFAULT_SPEED * movement.normalize();
    }

    static std::shared_ptr<Suzzane> Create(std::string id) {
        return std::make_shared<Suzzane>(id);
    }

    static const ActorCreatorEntry CREATORENTRY;
};

const float Suzzane::DEFAULT_SPEED = 2.0f;
const ActorCreatorEntry Suzzane::CREATORENTRY = std::make_pair("Suzzane", Suzzane::Create);


int main(int argc, char* argv[]) {
    engine = std::make_unique<Engine>(Engine::Create(1000, 1000, "Lawless", assetDir));
    engine->GetAssetManager().Register("glb", std::make_shared<gltf::GLBLoader>());
    engine->RegisterCreator(Suzzane::CREATORENTRY);

    engine->SetRenderer(std::make_shared<gl::Renderer>());
    engine->SetCamera(std::make_shared<Matrix4>(Matrix4::Perspective(110.0f, 1, 100)));

    auto scene = engine->GetScene();
    scene->transform.position = Vector3(0.0f, 0.0f, -30.0);

    auto suzanne = engine->Spawn<Suzzane>();
    suzanne->GetTransform().position = Vector3(0, 0.0, 0.0);
    scene->AddChild(suzanne->GetRoot());

    run();

    return 0;
}