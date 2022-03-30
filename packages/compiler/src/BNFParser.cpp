#include <vector>
#include <map>
#include "BNFParser.h"
#include "BNFTokenizer.h"

void BNFParser::Parse()
{
	this->token = this->tokenizer.Next();
	this->ParseProductions();
	assert(this->token.type == BNFTokenizer::TokenType::EndOfFile);
}

void BNFParser::ParseProductions()
{
	this->ParseProduction();
	if (this->token.type != BNFTokenizer::TokenType::EndOfFile) {
		this->ParseProductions();
	}
}

void BNFParser::ParseProduction()
{
	assert(this->token.type == BNFTokenizer::TokenType::Symbol);
	this->token = this->tokenizer.Next();
	assert(this->token.type == BNFTokenizer::TokenType::Replaces);
	this->token = this->tokenizer.Next();
	this->ParseExpression();
	assert(this->token.type == BNFTokenizer::TokenType::SemiColon);
	this->token = this->tokenizer.Next();
}

void BNFParser::ParseExpression()
{
	switch (this->token.type) {
		case BNFTokenizer::TokenType::Symbol:
		case BNFTokenizer::TokenType::Literal:
			this->token = this->tokenizer.Next();
			this->ParseExpressionPrime();
			break;
		default:
			assert(false);
	}
}

void BNFParser::ParseExpressionPrime()
{
	switch (this->token.type) {
		case BNFTokenizer::TokenType::Symbol:
		case BNFTokenizer::TokenType::Literal:
			this->token = this->tokenizer.Next();
			this->ParseExpressionPrime();
			break;
		case BNFTokenizer::TokenType::Alternate:
			this->token = this->tokenizer.Next();
			this->ParseExpressionPrime();
			break;
		default:
			break;
	}
}