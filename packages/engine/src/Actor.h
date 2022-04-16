#pragma once
#include <string>
#include <memory>

class SceneNode;
class ActorInitializer;
class Transform;
class Engine;

class Actor {
protected:
	std::shared_ptr<SceneNode> root{};
	Engine& engine;

public:
	std::string id;

	Actor(const ActorInitializer& initializer);
	virtual ~Actor() {};

	std::shared_ptr<SceneNode> GetRoot() { return root; }
	Transform& GetTransform();

	virtual void Tick(float deltaMs);
};