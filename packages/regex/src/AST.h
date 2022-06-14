#pragma once
#include <memory>
#include <optional>
#include <string>
#include <variant>

namespace regex {
namespace ast {

struct Node {
  virtual ~Node() {}
};

struct QuantifierNode : public Node {
  std::string quantifier;

  QuantifierNode(std::string quantifer) : quantifier(quantifer) {}
};

struct CharacterClassNode : public Node {
  bool negated{};
  std::string list{};
};

struct ValueNode : public Node {
  std::variant<char, std::unique_ptr<CharacterClassNode>> value;

  ValueNode(char character) : value(character) {}
  ValueNode(CharacterClassNode* characterClass)
    : value(std::unique_ptr<CharacterClassNode>(characterClass)) {}
};

struct ExpressionNode : public Node {
  std::unique_ptr<ValueNode> value;
  std::unique_ptr<QuantifierNode> quantifier;

  ExpressionNode(ValueNode* value, QuantifierNode* quantifier)
    : value(std::unique_ptr<ValueNode>(value))
    , quantifier(std::unique_ptr<QuantifierNode>(quantifier)) {}
};

struct RegularExpressionNode : public Node {
  std::vector<std::unique_ptr<ExpressionNode>> expressions{};

  RegularExpressionNode* Prepend(ExpressionNode* expression) {
    this->expressions.insert(this->expressions.begin(),
                             std::unique_ptr<ExpressionNode>(expression));
    return this;
  }
};

}
}
