#include "PlayerCamera.h"

const ActorCreatorEntry PlayerCamera::CREATORENTRY = Engine::RegisterClass("PlayerCamera", [](const ActorInitializer& initializer) { return std::make_shared<PlayerCamera>(initializer); });