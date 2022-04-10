#pragma once
#include <sstream>

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

const char* Parser = R"ESC(// This file was auto-generated
#include <assert.h>
#include <utility>
#include "Parser.generated.h"
{}

namespace {} {
{}
{}

};
)ESC";

const char* TokenizerIncludes = R"ESC(#include "RegularExpression.h"
#include <optional>)ESC";

const char* Tokenize = R"ESC(
Token Tokenize(std::stringstream& stream) {
	static std::vector<std::optional<TokenType>> tokens = {
{}};

	static DFA dfa = DFA::FromNFA(RegularExpression::Create(std::vector<std::string>({
{}})));

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
)ESC";

const char* ParserImplementation = R"ESC(
{} Parse{}(ParserContext& context) {
	switch(context.token.first) {
{}
		default:
			return nullptr;
	}
}

)ESC";

std::string format(std::string source, std::vector<std::string> replacements) {
	std::stringstream formatted{};
	size_t size = 0;
	size_t replace = 0;

	for (size_t i = 0; i < source.size() - 1; i++) {
		if (source[i] == '{' && source[i + 1] == '}') {
			formatted << source.substr(size, i - size);
			formatted << replacements[replace++];
			i = size = i + 2;
		}
	}

	formatted << source.substr(size, source.size() - size);

	return formatted.str();
}

};