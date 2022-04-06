#include <assert.h>
#include <utility>
#include "Parser.h"

namespace RegularExpression {

void* ParseOptionalQuantifier(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::Quantifier:
		{
			assert(context.token.first == TokenType::Quantifier);
			auto P0 = context.Use();

			{ return nullptr; }
		}
		default:
			return nullptr;
	}
}

void* ParseExpression(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::Character:
		{
			assert(context.token.first == TokenType::Character);
			auto P0 = context.Use();
			auto P1 = ParseOptionalQuantifier(context); 

			{ return nullptr; }
		}
		default:
			return nullptr;
	}
}

void* ParseOptionalSequence(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::Character:
		{
			auto P0 = ParseExpression(context); 
			auto P1 = ParseOptionalSequence(context); 

			{ return nullptr; }
		}
		default:
			return nullptr;
	}
}

void* ParseRegularExpression(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::Character:
		{
			auto P0 = ParseOptionalSequence(context); 
			assert(context.token.first == TokenType::EndOfFile);
			auto P1 = context.Use();

			{ return nullptr; }
		}
		default:
			return nullptr;
	}
}

}
