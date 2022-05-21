#pragma once
#include <sstream>

namespace fragments {

const char* Header = R"ESC(// This file was auto-generated
#pragma once
#include <sstream>
#include <functional>
#include <map>
{}
namespace {} {

enum class TokenType {
{}};

using Token = std::pair<TokenType, std::string>;

enum class ParserState {
	Default,
{}};

using Tokenizer = std::function<Token(std::stringstream& stream)>;
using Tokenizers = std::map<ParserState, Tokenizer>;

Tokenizers GetTokenizers();

class ParserContext {
public:
	std::stringstream stream;
	Tokenizers tokenizers;
	Tokenizer tokenize;
	Token token;

	ParserContext(std::string source, Tokenizers tokenizers)
		: stream(source), tokenizers(tokenizers), tokenize(tokenizers.at(ParserState::Default)), token(this->tokenize(this->stream)) { }

	Token Use() { auto old = this->token; this->token = this->tokenize(this->stream); return old; }
};

{}
};
)ESC";

const char* Parser = R"ESC(// This file was auto-generated
#include "Parser.generated.h"
#include <assert.h>
#include <utility>
{}

namespace {} {
{}
{}

};
)ESC";

const char* TokenizerIncludes = R"ESC(#include "regex.h"
#include <optional>)ESC";

const char* Tokenize = R"ESC(
Token Tokenize{}(std::stringstream& stream) {
	static std::vector<std::optional<TokenType>> tokens = {
{}};

	static DFA dfa = regex::Create(std::vector<std::string>({
{}}));

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

const char* GetTokenizers = R"ESC(
Tokenizers GetTokenizers() {
	static Tokenizers tokenizers{
		{}
	};

	return tokenizers;
}
)ESC";

const char* ParserImplementation = R"ESC(
{} Parse{}(ParserContext& context) {
	switch(context.token.first) {
{}
		default:
{}
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