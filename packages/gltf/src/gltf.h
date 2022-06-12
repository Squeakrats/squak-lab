#pragma once
#include <squak/core/AssetManager.h>
#include "SceneNode.h"
#include "json.h"

namespace gltf {

namespace binary {

const uint32_t GLTF = 0x46546C67;
const uint32_t JSON = 0x4E4F534A;
const uint32_t BIN = 0x004E4942;

};

std::shared_ptr<SceneNode> Parse(const std::vector<uint8_t>& buffer);
std::shared_ptr<SceneNode> Load(std::string path);

};