#pragma once
#include <variant>
#include <memory>
#include "BNFTokenizer.h"

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

class BNFParser {
private:
	BNFTokenizer tokenizer;
	BNFTokenizer::Token token{};

public:
	BNFParser(std::string source) : tokenizer(source) {}

	Grammar Parse();

private:
	std::vector<Production> ParseProductions();
	Production ParseProduction();
	Expression ParseExpression();
	Sequence ParseSequence();
	Value ParseValue();
};