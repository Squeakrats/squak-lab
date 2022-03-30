#include <vector>
#include <map>
#include "BNFParser.h"
#include "BNFTokenizer.h"

Grammar BNFParser::Parse()
{
	this->token = this->tokenizer.Next();

	Grammar grammar = this->ParseProductions();
	assert(this->token.type == BNFTokenizer::TokenType::EndOfFile);

	return grammar;
}

std::vector<Production> BNFParser::ParseProductions()
{
	std::vector<Production> productions{ this->ParseProduction() };

	if (this->token.type != BNFTokenizer::TokenType::EndOfFile) {
		std::vector<Production> tail = this->ParseProductions();
		productions.insert(productions.end(), tail.begin(), tail.end());
	}

	return productions;
}

Production BNFParser::ParseProduction()
{
	Production production{};

	assert(this->token.type == BNFTokenizer::TokenType::Symbol);
	production.symbol = Symbol{ this->token.text };
	this->token = this->tokenizer.Next();

	assert(this->token.type == BNFTokenizer::TokenType::Replaces);
	this->token = this->tokenizer.Next();

	production.expression = this->ParseExpression();

	assert(this->token.type == BNFTokenizer::TokenType::SemiColon);
	this->token = this->tokenizer.Next();

	return production;
}

Expression BNFParser::ParseExpression()
{
	Expression expression{ this->ParseSequence() };
	if (this->token.type == BNFTokenizer::TokenType::Alternate) {
		this->token = this->tokenizer.Next();

		Expression tail = this->ParseExpression();
		expression.insert(expression.end(), tail.begin(), tail.end());
	}

	return expression;
}

Sequence BNFParser::ParseSequence()
{
	Sequence sequence{ this->ParseValue() };
	if (this->token.type == BNFTokenizer::TokenType::Symbol || this->token.type == BNFTokenizer::TokenType::Literal) {
		Sequence tail = this->ParseSequence();
		sequence.insert(sequence.end(), tail.begin(), tail.end());
	}

	return sequence;
}

Value BNFParser::ParseValue()
{
	switch (this->token.type) {
		case BNFTokenizer::TokenType::Symbol: {
			Symbol symbol{ this->token.text };
			this->token = this->tokenizer.Next();

			return symbol;
		}
		case BNFTokenizer::TokenType::Literal: {
			Literal literal{ this->token.text };
			this->token = this->tokenizer.Next();

			return literal;
		}
		default:
			throw std::exception("unexpected value");
	}
}