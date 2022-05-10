#pragma once
#include <memory>
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

  RegularExpression(Sequence sequence)
    : sequence(sequence){};
};

struct CharacterClass {
  bool negated;
  std::string list;

  CharacterClass(bool negated, std::string list)
    : negated(negated)
    , list(list){};
};

using Value = std::variant<char, CharacterClass>;

struct Expression {
  Value value;
  std::optional<char> quantifier;

  Expression(Value value, std::optional<char> quantifier)
    : value(value)
    , quantifier(quantifier){};
};

}