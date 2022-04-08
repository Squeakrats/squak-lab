// This file was auto-generated 
#include <assert.h>
#include <utility>
#include "Parser.h"

namespace RegularExpression {

std::shared_ptr<AST::RegularExpression> ParseRegularExpression(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::Character:
		case TokenType::LeftBracket:
		{
			auto P0 = ParseOptionalSequence(context); 
			assert(context.token.first == TokenType::EndOfFile);
			auto P1 = context.Use();

			{ return std::make_shared<AST::RegularExpression>(P0); }
		}
		default:
			return nullptr;
	}
}

std::shared_ptr<AST::Sequence> ParseOptionalSequence(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::Character:
		case TokenType::LeftBracket:
		{
			auto P0 = ParseExpression(context); 
			auto P1 = ParseOptionalSequence(context); 

			{ return std::make_shared<AST::Sequence>(P0, P1); }
		}
		default:
			return nullptr;
	}
}

std::shared_ptr<AST::Expression> ParseExpression(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::Character:
		case TokenType::LeftBracket:
		{
			auto P0 = ParseValue(context); 
			auto P1 = ParseOptionalQuantifier(context); 

			{ return std::make_shared<AST::Expression>(P0, P1); }
		}
		default:
			return nullptr;
	}
}

std::shared_ptr<AST::Value> ParseValue(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::Character:
		{
			assert(context.token.first == TokenType::Character);
			auto P0 = context.Use();

			{ return std::make_shared<AST::Value>(std::make_shared<AST::Character>(P0.second)); }
		}
		case TokenType::LeftBracket:
		{
			auto P0 = ParseCharacterClass(context); 

			{ return std::make_shared<AST::Value>(P0); }
		}
		default:
			return nullptr;
	}
}

std::shared_ptr<AST::CharacterClass> ParseCharacterClass(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::LeftBracket:
		{
			assert(context.token.first == TokenType::LeftBracket);
			auto P0 = context.Use();
			auto P1 = ParseOptionalNot(context); 
			auto P2 = ParseCharacterClassList(context); 
			assert(context.token.first == TokenType::RightBracket);
			auto P3 = context.Use();

			{ return std::make_shared<AST::CharacterClass>(P1 != nullptr, P2); }
		}
		default:
			return nullptr;
	}
}

std::shared_ptr<AST::Not> ParseOptionalNot(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::Not:
		{
			assert(context.token.first == TokenType::Not);
			auto P0 = context.Use();

			{ return std::make_shared<AST::Not>(); }
		}
		default:
			return nullptr;
	}
}

std::shared_ptr<AST::CharacterClassList> ParseCharacterClassList(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::Character:
		{
			assert(context.token.first == TokenType::Character);
			auto P0 = context.Use();
			auto P1 = ParseCharacterClassList(context); 

			{ return std::make_shared<AST::CharacterClassList>(P0.second, P1); }
		}
		default:
			return nullptr;
	}
}

std::shared_ptr<AST::Quantifier> ParseOptionalQuantifier(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::Quantifier:
		{
			assert(context.token.first == TokenType::Quantifier);
			auto P0 = context.Use();

			{ return std::make_shared<AST::Quantifier>(P0.second); }
		}
		default:
			return nullptr;
	}
}

}
