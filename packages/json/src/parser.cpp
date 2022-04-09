// This file was auto-generated 
#include <assert.h>
#include <utility>
#include "Parser.h"
#include "RegularExpression.h"
#include <optional>

namespace json {

Token Tokenize(std::stringstream& stream) {
	static std::vector<std::optional<TokenType>> tokens = {
		 TokenType::LeftBrace,
		 TokenType::RightBrace,
		 TokenType::Colon,
		 TokenType::Comma,
		 TokenType::True,
		 TokenType::False,
		 TokenType::StringLiteral,
		 TokenType::NumberLiteral,
		 std::nullopt,
	};

	static DFA dfa = DFA::FromNFA(RegularExpression::Create(std::vector<std::string>({
		"{",
		"}",
		":",
		",",
		"true",
		"false",
		"\"[^\"]*\"",
		"[0123456789]+",
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
std::shared_ptr<ast::Object> Parsejson(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::LeftBrace:
		{
			auto P0 = ParseObject(context); 
			assert(context.token.first == TokenType::EndOfFile);
			auto P1 = context.Use();

			{ return P0; }
		}
		default:
			return nullptr;
	}
}

std::shared_ptr<ast::Object> ParseObject(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::LeftBrace:
		{
			assert(context.token.first == TokenType::LeftBrace);
			auto P0 = context.Use();
			auto P1 = ParseObjectEntries(context); 
			assert(context.token.first == TokenType::RightBrace);
			auto P2 = context.Use();

			{ return std::make_shared<ast::Object>(P1); }
		}
		default:
			return nullptr;
	}
}

std::shared_ptr<ast::ObjectEntries> ParseObjectEntries(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::StringLiteral:
		{
			auto P0 = ParseObjectEntry(context); 
			auto P1 = ParseOptionalComma(context); 
			auto P2 = ParseObjectEntries(context); 

			{ return std::make_shared<ast::ObjectEntries>(P0, P2); }
		}
		default:
			return nullptr;
	}
}

std::shared_ptr<ast::ObjectEntry> ParseObjectEntry(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::StringLiteral:
		{
			assert(context.token.first == TokenType::StringLiteral);
			auto P0 = context.Use();
			assert(context.token.first == TokenType::Colon);
			auto P1 = context.Use();
			auto P2 = ParseValue(context); 

			{ return std::make_shared<ast::ObjectEntry>(P0.second, P2); }
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

std::shared_ptr<ast::Value> ParseValue(ParserContext& context) { 
	switch(context.token.first) {
		case TokenType::True:
		{
			assert(context.token.first == TokenType::True);
			auto P0 = context.Use();

			{ return std::make_shared<ast::Value>(true); }
		}
		case TokenType::False:
		{
			assert(context.token.first == TokenType::False);
			auto P0 = context.Use();

			{ return std::make_shared<ast::Value>(false); }
		}
		case TokenType::StringLiteral:
		{
			assert(context.token.first == TokenType::StringLiteral);
			auto P0 = context.Use();

			{ return std::make_shared<ast::Value>(P0.second); }
		}
		case TokenType::NumberLiteral:
		{
			assert(context.token.first == TokenType::NumberLiteral);
			auto P0 = context.Use();

			{ return std::make_shared<ast::Value>(std::stod(P0.second)); }
		}
		default:
			return nullptr;
	}
}

}
