#pragma once
#include <string>
#include <optional>
#include <variant>

namespace regex::AST {

struct RegularExpression;
struct Expression;
using Sequence = std::vector<Expression>;
Sequence CreateSequence(Expression a, Sequence b);

struct RegularExpression {
  Sequence sequence;
};

struct CharacterClass {
  bool negated;
  std::string list;
};

using Value = std::variant<char, CharacterClass>;

struct Expression {
  Value value;
  std::optional<char> quantifier;
};

}