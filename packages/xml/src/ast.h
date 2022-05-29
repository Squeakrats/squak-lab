#pragma once
#include <list>
#include <map>
#include <string>
#include <variant>

namespace xml::ast {

enum class NodeType {
  Text,
  Attributes,
  Attribute,
  Elements,
  Element,
  Document
};

struct Node {
  NodeType type;

  Node(NodeType type) : type(type) {}
  virtual ~Node() {}
};

struct Attribute {
  std::string name;
  std::string value;
};

struct AttributesNode : public Node {
  std::vector<Attribute> value{};

  AttributesNode() : Node(NodeType::Attributes) {}

  static AttributesNode* Add(Attribute attribute, Node* b) {
    AttributesNode* attributesNode = static_cast<AttributesNode*>(b);
    attributesNode->value.push_back(attribute);

    return attributesNode;
  }
};

struct ElementsNode;

struct ElementNode : public Node {
  std::string name;
  std::unique_ptr<AttributesNode> attributes;
  std::unique_ptr<ElementsNode> children;

  ElementNode(std::string name,
              AttributesNode* attributes,
              ElementsNode* children)
    : Node(NodeType::Element)
    , name(name)
    , attributes(std::unique_ptr<AttributesNode>(attributes))
    , children(std::unique_ptr<ElementsNode>(children)) {}
};

struct TextNode : public Node {
  std::string value;

  TextNode(std::string value) : Node(NodeType::Text), value(value) {}
};

struct ElementsNode : public Node {
  std::vector<std::unique_ptr<Node>> value{};

  ElementsNode() : Node(NodeType::Elements) {}

  static ElementsNode* Add(Node* a, Node* b) {
    ElementsNode* elementsNode = static_cast<ElementsNode*>(b);

    elementsNode->value.push_back(std::unique_ptr<Node>(a));

    return elementsNode;
  }

  static ElementsNode* Add(std::string text, Node* b) {
    ElementsNode* elementsNode = static_cast<ElementsNode*>(b);
    elementsNode->value.push_back(std::make_unique<TextNode>(text));

    return elementsNode;
  }
};

struct DocumentNode : public Node {
  std::unique_ptr<ElementNode> root;

  DocumentNode(ElementNode* root)
    : Node(NodeType::Document), root(std::unique_ptr<ElementNode>(root)) {}
};

};