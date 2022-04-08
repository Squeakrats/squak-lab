#pragma once
#include <string>
#include <variant>
#include <map>

namespace json {

class Object;
using Value = std::variant<Object, std::string, bool>;

class Object {
public:
	std::map<std::string, Value> entries{};
};



Object Create(std::string source);

};