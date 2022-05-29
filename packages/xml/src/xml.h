#pragma once
#include <vector>
#include <map>
#include <memory>
#include <string>

namespace xml {

struct Node {
  virtual ~Node() {}
};

struct Element : public Node {
  std::string tag{};
  std::string text{};
  std::map<std::string, std::string> attributes{};
  std::vector<std::shared_ptr<Node>> children{};
};

struct TextNode : public Node {
  std::string text;

  TextNode(std::string text)
    : text(text) {}
};

struct Document {
  std::shared_ptr<Element> root;
};

Document Parse(std::string xml);

};
