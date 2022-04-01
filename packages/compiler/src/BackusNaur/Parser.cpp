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
	production.first = this->token.second;
	this->token = this->tokenizer.Next();

	assert(this->token.first == TokenType::Replaces);
	this->token = this->tokenizer.Next();

	production.second = this->ParseExpression();

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
	Sequence sequence{};
	if (this->token.first == TokenType::Symbol) {
		sequence.push_back(this->token.second);
		this->token = this->tokenizer.Next();

		Sequence tail = this->ParseSequence();
		sequence.insert(sequence.end(), tail.begin(), tail.end());
	}

	return sequence;
}

}
