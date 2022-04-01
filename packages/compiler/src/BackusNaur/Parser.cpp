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
	assert(this->token.first == TokenType::EndOfFile);

	return grammar;
}

std::vector<Production> Parser::ParseProductions()
{
	std::vector<Production> productions{ this->ParseProduction() };

	if (this->token.first != TokenType::EndOfFile) {
		std::vector<Production> tail = this->ParseProductions();
		productions.insert(productions.end(), tail.begin(), tail.end());
	}

	return productions;
}

Production Parser::ParseProduction()
{
	Production production{};

	assert(this->token.first == TokenType::Symbol);
	production.symbol = Symbol{ this->token.second };
	this->token = this->tokenizer.Next();

	assert(this->token.first == TokenType::Replaces);
	this->token = this->tokenizer.Next();

	production.expression = this->ParseExpression();

	assert(this->token.first == TokenType::SemiColon);
	this->token = this->tokenizer.Next();

	return production;
}

Expression Parser::ParseExpression()
{
	Expression expression{ this->ParseSequence() };
	if (this->token.first == TokenType::Alternate) {
		this->token = this->tokenizer.Next();

		Expression tail = this->ParseExpression();
		expression.insert(expression.end(), tail.begin(), tail.end());
	}

	return expression;
}

Sequence Parser::ParseSequence()
{
	Sequence sequence{ this->ParseValue() };
	if (this->token.first == TokenType::Symbol || this->token.first == TokenType::Literal) {
		Sequence tail = this->ParseSequence();
		sequence.insert(sequence.end(), tail.begin(), tail.end());
	}

	return sequence;
}

Value Parser::ParseValue()
{
	switch (this->token.first) {
		case TokenType::Symbol: {
			Symbol symbol{ this->token.second };
			this->token = this->tokenizer.Next();

			return symbol;
		}
		case TokenType::Literal: {
			Literal literal{ this->token.second };
			this->token = this->tokenizer.Next();

			return literal;
		}
		default:
			throw std::exception("unexpected value");
	}
}

}
