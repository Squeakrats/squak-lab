#include "Actor.h"
#include "Engine.h"

Actor::Actor(const ActorInitializer& initializer)
  : id(initializer.id)
  , root(std::make_shared<SceneNode>(initializer.transform))
  , engine(initializer.engine) {}

Transform& Actor::GetTransform() {
  return this->root->transform;
}

void Actor::Tick(float deltaMs) {
  // nothing to do
}