#pragma once
#include <string>
#include "SceneNode.h"

class Actor {
protected:
	std::shared_ptr<SceneNode> root{};

public:
	std::string id;

	Actor(std::string id) : id(id) {};
	virtual ~Actor() {};

	std::shared_ptr<SceneNode> GetRoot() { return root; }
	virtual void Tick(float deltaMs);
};