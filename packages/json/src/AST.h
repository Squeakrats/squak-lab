#pragma once
#include <memory>

namespace JSON::AST {

struct Object;
struct ObjectEntries;
struct ObjectEntry;

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

	ObjectEntry(std::string key) : key(key) {};
};

};