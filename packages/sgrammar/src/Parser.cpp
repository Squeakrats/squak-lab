#include <assert.h>
#include <vector>
#include <map>
#include "Parser.h"
#include "Tokenizer.h"

AST::Grammar Parser::Parse()
{
	this->token = this->source.Next();

	AST::Grammar grammar{};
	assert(this->token.first == TokenType::Code);
	grammar.code = this->Use().second;

	grammar.productions = this->ParseProductions();
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
	production.symbol = this->Use().second;

	assert(this->Use().first == TokenType::Replaces);

	production.expression = this->ParseExpression();
	assert(this->Use().first == TokenType::SemiColon);

	assert(this->token.first == TokenType::Code);
	production.type = this->Use().second;

	assert(this->token.first == TokenType::Code);
	production.code = this->Use().second;

	assert(this->Use().first == TokenType::SemiColon);

	return production;
}

AST::Expression Parser::ParseExpression()
{
	AST::Expression expression{ this->ParseSequence() };
	if (this->token.first == TokenType::Alternate) {
		this->Use();

		AST::Expression tail = this->ParseExpression();
		expression.insert(expression.end(), tail.begin(), tail.end());
	}

	return expression;
}

AST::Sequence Parser::ParseSequence()
{
	AST::Sequence sequence{};
	if (this->token.first == TokenType::Symbol) {
		sequence.symbols.push_back(this->Use().second);

		AST::Sequence tail = this->ParseSequence();
		sequence.symbols.insert(sequence.symbols.end(), tail.symbols.begin(), tail.symbols.end());
	}

	return sequence;
}
