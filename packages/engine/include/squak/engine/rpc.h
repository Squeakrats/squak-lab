#pragma once
#include <string>
#include <vector>
#include <variant>

class RPC {
public:
  using Argument = std::variant<double, bool, std::string>;

  std::string id{};
  std::string method{};
  std::vector<Argument> arguments{};
};