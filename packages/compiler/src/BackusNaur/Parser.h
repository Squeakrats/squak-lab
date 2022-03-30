#pragma once
#include <vector>
#include <variant>
#include "Tokenizer.h"

/*
<grammer> ::= <productions>

<productions> ::= <production>
				| <production> <productions>

<production> ::= <symbol> <derives> <expression> <semicolon>

<expression> ::= <sequence>
               | <sequence> <alternate> <expression>

<sequence>	::= <value>
			 | <value> | <sequence>

<value> ::= <symbol> | <literal>
*/

namespace BackusNaur {

struct Symbol {
	std::string name{};
};

struct Literal {
	std::string value{};
};

using Value = std::variant<Symbol, Literal>;
using Sequence = std::vector<Value>;
using Expression = std::vector<Sequence>;

struct Production {
	Symbol symbol{};
	Expression expression{};
};

using Grammar = std::vector<Production>;

class Parser {
private:
	Tokenizer tokenizer;
	Token token{};

public:
	Parser(std::string source) : tokenizer(source) {}

	Grammar Parse();

private:
	std::vector<Production> ParseProductions();
	Production ParseProduction();
	Expression ParseExpression();
	Sequence ParseSequence();
	Value ParseValue();
};

}
