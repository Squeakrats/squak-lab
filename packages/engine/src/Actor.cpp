#include "Actor.h"
#include "Engine.h"

Actor::Actor(const ActorInitializer& initializer)
	: id(initializer.id), engine(initializer.engine) {}

Transform& Actor::GetTransform() {
	return this->root->transform;
}

void Actor::Tick(float deltaMs) {
	// nothing to do
}