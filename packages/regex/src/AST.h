#pragma once
#include <optional>
#include <string>
#include <variant>
#include <memory>

namespace regex {
namespace ast {

enum class NodeType {
  RegularExpression,
  Expression,
  Sequence,
  Character,
  CharacterClass,
  Quantifier,
  Value
};

struct Node {
  NodeType type;

  Node(NodeType type) : type(type) {}
  virtual ~Node() {}
};

struct QuantifierNode : public Node {
  std::string quantifier;

  QuantifierNode(std::string quantifer)
    : Node(NodeType::Quantifier), quantifier(quantifer) {}
};

struct CharacterClassNode : public Node {
  bool negated{};
  std::string list{};

  CharacterClassNode() : Node(NodeType::CharacterClass) {}

  static CharacterClassNode* Add(char character, Node* node) {
    ast::CharacterClassNode* classNode =
      static_cast<ast::CharacterClassNode*>(node);

    classNode->list += character;

    return classNode;
  }

  static CharacterClassNode* Negate(Node* node) {
    ast::CharacterClassNode* classNode =
      static_cast<ast::CharacterClassNode*>(node);

    classNode->negated = true;

    return classNode;
  }
};

struct ValueNode : public Node {
  std::variant<char, std::unique_ptr<CharacterClassNode>> value;

  ValueNode(char character) : Node(NodeType::Value), value(character) {}
  ValueNode(CharacterClassNode* characterClass)
    : Node(NodeType::Value), value(std::unique_ptr<CharacterClassNode>(characterClass)) {}

  static ValueNode* Create(Node* node) {
    return new ValueNode(static_cast<CharacterClassNode*>(node));
  }
};

struct ExpressionNode : public Node {
  std::unique_ptr<ValueNode> value;
  std::unique_ptr<QuantifierNode> quantifier;

  ExpressionNode(ValueNode* value, QuantifierNode* quantifier)
    : Node(NodeType::Expression)
    , value(std::unique_ptr<ValueNode>(value))
    , quantifier(std::unique_ptr<QuantifierNode>(quantifier)) {}

  static ExpressionNode* Create(Node* value, Node* quantifier) {
    return new ast::ExpressionNode(static_cast<ValueNode*>(value),
                                   static_cast<QuantifierNode*>(quantifier));
  }
};

struct RegularExpressionNode : public Node {
  std::vector<ExpressionNode*> expressions{};

  RegularExpressionNode* InsertFront(ExpressionNode* expression) {
    this->expressions.insert(this->expressions.begin(), expression);
    return this;
  }

  RegularExpressionNode() : Node(NodeType::RegularExpression) {}

  static RegularExpressionNode* Add(Node* expressionNode, Node* regexNode) {
    RegularExpressionNode* regex =
      static_cast<RegularExpressionNode*>(regexNode);

    regex->expressions.insert(regex->expressions.begin(),
                              static_cast<ExpressionNode*>(expressionNode));

    return regex;
  }
};

}
}
