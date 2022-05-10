#pragma once
#include <memory>
#include <string>
#include <variant>

namespace json::ast {

struct Object;
struct Array;
using Value = std::variant<std::shared_ptr<ast::Object>,
                           std::shared_ptr<ast::Array>,
                           double,
                           bool,
                           std::string>;

struct ObjectEntry {
  std::string key{};
  Value value{};
};

struct Object {
  std::list<ObjectEntry> entries{};

  static std::list<ast::ObjectEntry> Create(ast::ObjectEntry a,
                                            std::list<ast::ObjectEntry> b) {
    std::list<ast::ObjectEntry> value = std::list<ast::ObjectEntry>({ a });
    value.insert(value.end(), b.begin(), b.end());

    return value;
  }
};

struct Array {
  std::list<ast::Value> elements;

  static Array Create(ast::Value a, ast::Array b) {
    ast::Array value = ast::Array({{ a }});
    value.elements.insert(
      value.elements.end(), b.elements.begin(), b.elements.end());

    return value;
  }
};

};