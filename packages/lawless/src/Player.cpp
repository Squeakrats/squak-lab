#include "Player.h"

const ActorCreatorEntry Player::CREATORENTRY = std::make_pair("Player", [](const ActorInitializer& initializer) { return std::make_shared<Player>(initializer); });
