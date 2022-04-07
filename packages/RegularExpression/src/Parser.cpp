#include <assert.h>
#include <utility>
#include "Parser.h"

namespace RegularExpression {

AST::RegularExpression* ParseRegularExpression(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::Character:
		case TokenType::LeftBracket:
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
		case TokenType::LeftBracket:
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
		case TokenType::LeftBracket:
		{
			auto P0 = ParseValue(context); 
			auto P1 = ParseOptionalQuantifier(context); 

			{ return new AST::Expression(P0, P1); }
		}
		default:
			return nullptr;
	}
}

AST::Value* ParseValue(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::Character:
		{
			assert(context.token.first == TokenType::Character);
			auto P0 = context.Use();

			{ return new AST::Value(new AST::Character(P0.second)); }
		}
		case TokenType::LeftBracket:
		{
			auto P0 = ParseCharacterClass(context); 

			{ return new AST::Value(P0); }
		}
		default:
			return nullptr;
	}
}

AST::CharacterClass* ParseCharacterClass(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::LeftBracket:
		{
			assert(context.token.first == TokenType::LeftBracket);
			auto P0 = context.Use();
			auto P1 = ParseOptionalNot(context); 
			auto P2 = ParseCharacterClassList(context); 
			assert(context.token.first == TokenType::RightBracket);
			auto P3 = context.Use();

			{ return new AST::CharacterClass(P1, P2); }
		}
		default:
			return nullptr;
	}
}

AST::Not* ParseOptionalNot(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::Not:
		{
			assert(context.token.first == TokenType::Not);
			auto P0 = context.Use();

			{ return new AST::Not(); }
		}
		default:
			return nullptr;
	}
}

AST::CharacterClassList* ParseCharacterClassList(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::Character:
		{
			assert(context.token.first == TokenType::Character);
			auto P0 = context.Use();
			auto P1 = ParseCharacterClassList(context); 

			{ return new AST::CharacterClassList(P0.second, P1); }
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
