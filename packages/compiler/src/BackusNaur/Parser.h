#pragma once
#include <vector>
#include <variant>
#include "TokenStream.h"
#include "Tokenizer.h"

/*
<grammer> ::= <productions>

<productions> ::= <production>
				| <production> <productions>

<production> ::= <symbol> <derives> <expression> <semicolon>

<expression> ::= <sequence>
               | <sequence> <alternate> <expression>

<sequence>	::= <symbol>
			 | <symbol> | <sequence>
*/

namespace BackusNaur {

namespace AST {

using Symbol = std::string;
using Sequence = std::vector<Symbol>;
using Expression = std::vector<Sequence>;
using Production = std::pair<Symbol, Expression>;
using Grammar = std::vector<Production>;

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

}
