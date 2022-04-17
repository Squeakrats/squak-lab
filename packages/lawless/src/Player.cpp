#include "Player.h"

const ActorCreatorEntry Player::CREATORENTRY = Engine::RegisterClass("Player", [](const ActorInitializer& initializer) { return std::make_shared<Player>(initializer); });