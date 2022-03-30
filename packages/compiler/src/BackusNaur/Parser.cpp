#include <assert.h>
#include <vector>
#include <map>
#include "Parser.h"
#include "Tokenizer.h"

namespace BackusNaur {

Grammar Parser::Parse()
{
	this->token = this->tokenizer.Next();

	Grammar grammar = this->ParseProductions();
	assert(this->token.type == TokenType::EndOfFile);

	return grammar;
}

std::vector<Production> Parser::ParseProductions()
{
	std::vector<Production> productions{ this->ParseProduction() };

	if (this->token.type != TokenType::EndOfFile) {
		std::vector<Production> tail = this->ParseProductions();
		productions.insert(productions.end(), tail.begin(), tail.end());
	}

	return productions;
}

Production Parser::ParseProduction()
{
	Production production{};

	assert(this->token.type == TokenType::Symbol);
	production.symbol = Symbol{ this->token.text };
	this->token = this->tokenizer.Next();

	assert(this->token.type == TokenType::Replaces);
	this->token = this->tokenizer.Next();

	production.expression = this->ParseExpression();

	assert(this->token.type == TokenType::SemiColon);
	this->token = this->tokenizer.Next();

	return production;
}

Expression Parser::ParseExpression()
{
	Expression expression{ this->ParseSequence() };
	if (this->token.type == TokenType::Alternate) {
		this->token = this->tokenizer.Next();

		Expression tail = this->ParseExpression();
		expression.insert(expression.end(), tail.begin(), tail.end());
	}

	return expression;
}

Sequence Parser::ParseSequence()
{
	Sequence sequence{ this->ParseValue() };
	if (this->token.type == TokenType::Symbol || this->token.type == TokenType::Literal) {
		Sequence tail = this->ParseSequence();
		sequence.insert(sequence.end(), tail.begin(), tail.end());
	}

	return sequence;
}

Value Parser::ParseValue()
{
	switch (this->token.type) {
		case TokenType::Symbol: {
			Symbol symbol{ this->token.text };
			this->token = this->tokenizer.Next();

			return symbol;
		}
		case TokenType::Literal: {
			Literal literal{ this->token.text };
			this->token = this->tokenizer.Next();

			return literal;
		}
		default:
			throw std::exception("unexpected value");
	}
}

}
