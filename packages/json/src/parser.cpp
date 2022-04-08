// This file was auto-generated 
#include <assert.h>
#include <utility>
#include "Parser.h"
#include "RegularExpression.h"
#include <optional>

namespace JSON {

Token Tokenize(std::stringstream& stream) {
	static std::vector<std::optional<TokenType>> tokens = {
		 TokenType::LeftBrace,
		 TokenType::RightBrace,
		 TokenType::Colon,
		 TokenType::StringLiteral,
		 std::nullopt,
	};

	static DFA dfa = DFA::FromNFA(RegularExpression::Create(std::vector<std::string>({
		"{",
		"}",
		":",
		"\"[^\"]*\"",
		"[\t\n ]",
	})));

	while(true) {
		auto longest = dfa.Longest(stream);
		if (longest.second != 0) {
			std::optional<TokenType> token = tokens[longest.second - 1];
			if (token != std::nullopt) {
				return std::make_pair(token.value(), longest.first); 
			}
		} else {
			return std::make_pair(TokenType::EndOfFile, "");
		}
	}
}
void* ParseJSON(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::LeftBrace:
		{
			auto P0 = ParseObject(context); 
			assert(context.token.first == TokenType::EndOfFile);
			auto P1 = context.Use();

			{ return nullptr; }
		}
		default:
			return nullptr;
	}
}

void* ParseObject(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::LeftBrace:
		{
			assert(context.token.first == TokenType::LeftBrace);
			auto P0 = context.Use();
			auto P1 = ParseObjectEntries(context); 
			assert(context.token.first == TokenType::RightBrace);
			auto P2 = context.Use();

			{ return nullptr; }
		}
		default:
			return nullptr;
	}
}

void* ParseObjectEntries(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::StringLiteral:
		{
			auto P0 = ParseObjectEntry(context); 
			auto P1 = ParseOptionalComma(context); 
			auto P2 = ParseObjectEntries(context); 

			{ return nullptr; }
		}
		default:
			return nullptr;
	}
}

void* ParseObjectEntry(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::StringLiteral:
		{
			assert(context.token.first == TokenType::StringLiteral);
			auto P0 = context.Use();
			assert(context.token.first == TokenType::Colon);
			auto P1 = context.Use();
			auto P2 = ParseValue(context); 

			{ return nullptr; }
		}
		default:
			return nullptr;
	}
}

void* ParseOptionalComma(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::Comma:
		{
			assert(context.token.first == TokenType::Comma);
			auto P0 = context.Use();

			{ return nullptr; }
		}
		default:
			return nullptr;
	}
}

void* ParseValue(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::True:
		{
			assert(context.token.first == TokenType::True);
			auto P0 = context.Use();

			{ return nullptr; }
		}
		case TokenType::False:
		{
			assert(context.token.first == TokenType::False);
			auto P0 = context.Use();

			{ return nullptr; }
		}
		case TokenType::StringLiteral:
		{
			assert(context.token.first == TokenType::StringLiteral);
			auto P0 = context.Use();

			{ return nullptr; }
		}
		case TokenType::NumberLiteral:
		{
			assert(context.token.first == TokenType::NumberLiteral);
			auto P0 = context.Use();

			{ return nullptr; }
		}
		default:
			return nullptr;
	}
}

}
