#pragma once
#include <assert.h>
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

	Value& operator[](std::string key) {
		assert(std::holds_alternative<Object>(this->value));

		return std::get<Object>(this->value)[key];
	}

	Value& operator[](size_t index) {
		assert(std::holds_alternative<std::vector<Value>>(this->value));

		return std::get<std::vector<Value>>(this->value)[index];
	}
};

Object Parse(std::string source);

};