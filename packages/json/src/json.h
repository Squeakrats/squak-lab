#pragma once
#include <assert.h>
#include <string>
#include <variant>
#include <vector>
#include <map>

namespace json {

class Object;
class Value;
using Array = std::vector<Value>;

class Object {
public:
	std::map<std::string, Value> entries{};

	Value& operator[](std::string key){
		return this->entries.at(key);
	}

	bool has(std::string key);
};

class Value {
public:
	std::variant<Object, Array, std::string, bool, double> value;

	template<typename T>
	T& get() { return std::get<T>(this->value); };

	template<typename T>
	T as() { return static_cast<T>(std::get<double>(this->value)); };

	Value& operator[](std::string key) {
		return std::get<Object>(this->value)[key];
	}

	Value& operator[](size_t index) {
		return std::get<Array>(this->value)[index];
	}
};

Object Parse(std::string source);

};