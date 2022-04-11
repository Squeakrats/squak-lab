#include <assert.h>
#include <vector>
#include <map>
#include "Parser.h"
#include "Tokenizer.h"

AST::Grammar Parser::Parse()
{
	this->token = this->source.Next();

	AST::Grammar grammar{};
	grammar.code = this->Use(TokenType::Code).second;

	if (this->token.first == TokenType::LeftBracket) {
		this->Use(TokenType::LeftBracket);
		while (this->token.first == TokenType::Symbol) {
			std::string token = this->Use(TokenType::Symbol).second;
			this->Use(TokenType::Replaces).first;
			std::string expression = this->Use(TokenType::Symbol).second;
			this->Use(TokenType::SemiColon);

			grammar.tokens.push_back(std::make_pair(token, expression));
		}

		this->Use(TokenType::RightBracket);
	}

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

	production.symbol = this->Use(TokenType::Symbol).second;
	production.type = this->Use(TokenType::Code).second;
	this->Use(TokenType::Replaces);
	production.expression = this->ParseExpression();
	this->Use(TokenType::SemiColon);

	return production;
}

AST::Expression Parser::ParseExpression()
{
	AST::Expression expression{ this->ParseSequence() };
	if (this->token.first == TokenType::Alternate) {
		this->Use(TokenType::Alternate);

		AST::Expression tail = this->ParseExpression();
		expression.insert(expression.end(), tail.begin(), tail.end());
	}

	return expression;
}

AST::Sequence Parser::ParseSequence()
{
	AST::Sequence sequence{};
	while (this->token.first == TokenType::Symbol) {
		sequence.symbols.push_back(this->Use(TokenType::Symbol).second);
	}

	sequence.code = this->Use(TokenType::Code).second;

	return sequence;
}
