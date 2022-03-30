#pragma once
#include "BNFTokenizer.h"

/*
<grammer> ::= <productions>
<productions> ::= <production>
				| <production> <productions>

<production> ::= <symbol> "::=" <expression> <semicolon>

<expression> ::= <symbol> <expression'>
			   | <literal> <expression'>

<expression'> ::= <symbol> <expression'>
				| <literal> <expression'>
				| <alternate> <expression'>
*/

class BNFParser {
private:
	BNFTokenizer tokenizer;
	BNFTokenizer::Token token{};

public:
	BNFParser(std::string source) : tokenizer(source) {}

	void Parse();

private:
	void ParseProductions();
	void ParseProduction();
	void ParseExpression();
	void ParseExpressionPrime();
};