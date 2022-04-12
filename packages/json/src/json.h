#pragma once
#include <string>
#include <variant>
#include <vector>
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
	std::variant<Object, std::vector<Value>, std::string, bool, double> value;

	template<typename T>
	T& get() { return std::get<T>(this->value); };
};

Object Parse(std::string source);

};