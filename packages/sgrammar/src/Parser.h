#pragma once
#include <vector>
#include <variant>
#include "TokenStream.h"
#include "Tokenizer.h"

/*
<grammer> ::= <code> <productions>

<productions> ::= <production>
				| <production> <productions>

<production> ::= <symbol> <derives> <expression> <semicolon> <code>

<expression> ::= <sequence>
               | <sequence> <alternate> <expression>

<sequence>	::= <symbol>
			 | <symbol> | <sequence>
*/

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
	std::vector<Production> productions{};
};

};

class Parser {
private:
	TokenStream<Token> source;
	Token token{};

public:
	Parser(std::string source) : source(source, Tokenize) {}

	static AST::Grammar Parse(std::string source) {
		Parser parser(source);

		return parser.Parse();
	}

private:
	Token Use() {
		Token token = this->token;
		this->token = this->source.Next();

		return token;
	}

	AST::Grammar Parse();
	std::vector<AST::Production> ParseProductions();
	AST::Production ParseProduction();
	AST::Expression ParseExpression();
	AST::Sequence ParseSequence();
};
