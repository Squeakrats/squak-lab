#pragma once
#include <list>
#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace json::ast {

enum class NodeType { Bool, String, Double, Array, Object, ObjectEntry };

struct Node {
  NodeType type;

  Node(NodeType type) : type(type) {}
  virtual ~Node() {}
};

struct BoolNode : public Node {
  bool value;

  BoolNode(bool value) : Node(NodeType::Bool), value(value) {}
};

struct StringNode : public Node {
  std::string value;

  StringNode(std::string value) : Node(NodeType::String), value(value) {}
};

struct DoubleNode : public Node {
  double value;

  DoubleNode(double value) : Node(NodeType::Double), value(value) {}
};

struct ObjectEntryNode : public Node {
  std::string key;
  std::unique_ptr<Node> value;

  ObjectEntryNode(std::string key, Node* value)
    : Node(NodeType::ObjectEntry)
    , key(key)
    , value(std::unique_ptr<Node>(value)) {}
};

struct ObjectNode : public Node {
  std::vector<std::unique_ptr<ObjectEntryNode>> value{};

  ObjectNode() : Node(NodeType::Object) {}

  ObjectNode* Add(ObjectEntryNode* a) {
    this->value.push_back(std::unique_ptr<ObjectEntryNode>(a));

    return this;
  }
};

struct ArrayNode : public Node {
  std::vector<std::unique_ptr<Node>> value{};

  ArrayNode() : Node(NodeType::Array) {}

  ArrayNode* Add(Node* a) {
    this->value.push_back(std::unique_ptr<Node>(a));

    return this;
  }
};

};