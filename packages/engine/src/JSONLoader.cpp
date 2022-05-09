#include "JSONLoader.h"
#include "json.h"
#include "utility.h"
#include <fstream>
#include <sstream>

std::shared_ptr<IAsset> JSONLoader::Load(std::string name) {
  std::ifstream file(name, std::ios::binary);
  Assert(file.is_open(), "file must be valid to read");

  std::stringstream buffer;
  buffer << file.rdbuf();

  return std::make_shared<JSONAsset>(json::Parse(buffer.str()));
}