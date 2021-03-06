#pragma once
#include "Tokenizer.h"
#include "utility.h"
#include <variant>
#include <vector>
#include <functional>
#include <map>

namespace AST {

using Symbol = std::string;

struct Sequence {
  std::vector<Symbol> symbols{};
  std::string code{};
};

using Expression = std::vector<Sequence>;

struct Production {
  Symbol symbol{};
  std::string type{};
  Expression expression{};
};

struct Grammar {
  std::string code{};
  std::map<std::string, std::vector<std::pair<std::string, std::string>>> tokens{};
  std::vector<Production> productions{};
};

};

class Parser {
private:
  std::stringstream source;
  Token token{};

public:
  Parser(std::string source)
    : source(source) {}

  static AST::Grammar Parse(std::string source) {
    Parser parser(source);

    return parser.Parse();
  }

private:
  Token Use(TokenType type) {
    Assert(this->token.first == type, "unhandled token");

    Token token = this->token;
    this->token = Tokenize(this->source);

    return token;
  }

  AST::Grammar Parse();
  std::vector<AST::Production> ParseProductions();
  AST::Production ParseProduction();
  AST::Expression ParseExpression();
  AST::Sequence ParseSequence();
};
