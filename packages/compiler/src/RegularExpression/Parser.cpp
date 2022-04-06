#include <assert.h>
#include <utility>
#include "Parser.h"

namespace RegularExpression {

AST::RegularExpression* ParseRegularExpression(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::Character:
		{
			auto P0 = ParseOptionalSequence(context); 
			assert(context.token.first == TokenType::EndOfFile);
			auto P1 = context.Use();

			{ return new AST::RegularExpression(P0); }
		}
		default:
			return nullptr;
	}
}

AST::Sequence* ParseOptionalSequence(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::Character:
		{
			auto P0 = ParseExpression(context); 
			auto P1 = ParseOptionalSequence(context); 

			{ return new AST::Sequence(P0, P1); }
		}
		default:
			return nullptr;
	}
}

AST::Expression* ParseExpression(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::Character:
		{
			assert(context.token.first == TokenType::Character);
			auto P0 = context.Use();
			auto P1 = ParseOptionalQuantifier(context); 

			{ return new AST::Expression(P0.second, P1); }
		}
		default:
			return nullptr;
	}
}

AST::Quantifier* ParseOptionalQuantifier(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::Quantifier:
		{
			assert(context.token.first == TokenType::Quantifier);
			auto P0 = context.Use();

			{ return new AST::Quantifier(P0.second); }
		}
		default:
			return nullptr;
	}
}

}
