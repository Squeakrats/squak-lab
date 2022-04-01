#include <assert.h>
#include <vector>
#include <map>
#include "Parser.h"
#include "Tokenizer.h"

namespace BackusNaur {

AST::Grammar Parser::Parse()
{
	this->token = this->tokenizer.Next();

	AST::Grammar grammar = this->ParseProductions();
	assert(this->token.first == TokenType::EndOfFile);

	return grammar;
}

std::vector<AST::Production> Parser::ParseProductions()
{
	std::vector<AST::Production> productions{ this->ParseProduction() };

	if (this->token.first != TokenType::EndOfFile) {
		std::vector<AST::Production> tail = this->ParseProductions();
		productions.insert(productions.end(), tail.begin(), tail.end());
	}

	return productions;
}

AST::Production Parser::ParseProduction()
{
	AST::Production production{};

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

AST::Expression Parser::ParseExpression()
{
	AST::Expression expression{ this->ParseSequence() };
	if (this->token.first == TokenType::Alternate) {
		this->token = this->tokenizer.Next();

		AST::Expression tail = this->ParseExpression();
		expression.insert(expression.end(), tail.begin(), tail.end());
	}

	return expression;
}

AST::Sequence Parser::ParseSequence()
{
	AST::Sequence sequence{};
	if (this->token.first == TokenType::Symbol) {
		sequence.push_back(this->token.second);
		this->token = this->tokenizer.Next();

		AST::Sequence tail = this->ParseSequence();
		sequence.insert(sequence.end(), tail.begin(), tail.end());
	}

	return sequence;
}

}
