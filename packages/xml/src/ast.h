#pragma once
#include <list>
#include <map>
#include <string>
#include <variant>

namespace xml::ast {

struct Attribute {
  std::string name;
  std::string value;
};

inline std::list<Attribute> CreateAttribute(Attribute a,
                                            std::list<Attribute> b) {
  std::list<Attribute> attributes{ a };
  attributes.insert(attributes.end(), b.begin(), b.end());

  return attributes;
}

struct OpenTag {
  std::string tagName;
  std::list<Attribute> attributes;
};

struct Node {
  virtual ~Node(){};
};

struct TextNode : public Node {
  std::string text;

  TextNode(std::string text)
    : text(text) {}
};

struct Element : public Node {
  OpenTag openTag;
  std::list<std::shared_ptr<Node>> children;

  Element(OpenTag openTag, std::list<std::shared_ptr<Node>> children)
    : openTag(openTag)
    , children(children) {}
};

inline std::list<std::shared_ptr<Node>> CreateNode(
  std::shared_ptr<Node> a,
  std::list<std::shared_ptr<Node>> b) {
  std::list<std::shared_ptr<Node>> nodes{ a };
  nodes.insert(nodes.end(), b.begin(), b.end());

  return nodes;
}

struct Document {
  std::shared_ptr<Element> root;
};

};