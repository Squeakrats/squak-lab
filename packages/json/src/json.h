#pragma once
#include <string>
#include <variant>
#include <map>

namespace json {

class Object;
class Value;

class Object {
public:
	std::map<std::string, Value> entries{};

	Value& operator[](std::string key){
		return this->entries.at(key);
	}
};

class Value {
public:
	std::variant<Object, std::string, bool> value;

	std::string AsString() { return std::get<std::string>(this->value); }
	bool AsBool() { return std::get<bool>(this->value); }
};

Object Create(std::string source);

};