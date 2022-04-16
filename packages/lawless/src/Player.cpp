#include "Player.h"

const float Player::DEFAULT_SPEED = 2.0f;
const ActorCreatorEntry Player::CREATORENTRY = std::make_pair("Player", [](const ActorInitializer& initializer) { return std::make_shared<Player>(initializer); });
