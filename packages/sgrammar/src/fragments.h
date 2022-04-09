#pragma once

namespace fragments {

const char* Header = R"ESC(// This file was auto-generated
#pragma once
#include "TokenStream.h"
{}
namespace {} {

enum class TokenType {
{}};

using Token = std::pair<TokenType, std::string>;

Token Tokenize(std::stringstream& source);

class ParserContext {
public:
	Token token;
	TokenStream<Token>& stream;
	Token Use() { auto old = this->token; this->token = this->stream.Next(); return old; }
};

{}
};
)ESC";

const char* NextToken =
R"ESC(	while(true) {
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
)ESC";

};