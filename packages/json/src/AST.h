#pragma once
#include <memory>
#include <variant>
#include <string>

namespace json::ast {

struct Object;
struct ObjectEntries;
struct ObjectEntry;
struct Value;

struct Object {
	std::shared_ptr<ObjectEntries> entries{};

	Object(std::shared_ptr<ObjectEntries> entries) : entries(entries) {};
};

struct ObjectEntries {
	std::shared_ptr<ObjectEntry> entry;
	std::shared_ptr<ObjectEntries> rhs;

	ObjectEntries(std::shared_ptr<ObjectEntry> entry, std::shared_ptr<ObjectEntries> rhs) : entry(entry), rhs(rhs) {};
};

struct ObjectEntry {
	std::string key{};
	std::shared_ptr<Value> value{};

	ObjectEntry(std::string key, std::shared_ptr<Value> value) : key(key), value(value) {};
};

struct Value {
	std::variant<std::shared_ptr<ast::Object>, double, bool, std::string> value;

	Value(std::variant<std::shared_ptr<ast::Object>, double, bool, std::string> value) : value(value) {};
};

};