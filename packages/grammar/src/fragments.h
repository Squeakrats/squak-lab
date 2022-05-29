#pragma once
#include <sstream>

namespace fragments {

const char* Header = R"ESC(// This file was auto-generated
#pragma once
#include <sstream>
#include <functional>
#include <map>
#include <stack>
#include <optional>
{}
namespace {} {

enum class TokenType {
{}};

enum class ProductionType {
{}};

using Token = std::pair<TokenType, std::string>;
using Symbol = std::variant<TokenType, ProductionType>;
class ParserContext;

struct Alternation {
	std::vector<Symbol> symbols;
	std::function<std::variant<Token, void*>(ParserContext& context, std::vector<std::variant<Token, void*>>)> code;
};

struct Production {
  std::map<TokenType, size_t> first;
  std::vector<Alternation> alternations;
};

enum class ParserState {
	Default,
{}};

using Tokenizer = std::function<Token(std::stringstream& stream)>;
using Tokenizers = std::map<ParserState, Tokenizer>;

Tokenizers GetTokenizers();

std::vector<Production> GetParseTable();

class ParserContext {
public:
	std::stack<ParserState> state{ };
	std::stringstream stream;
	Tokenizers tokenizers;
	Tokenizer tokenize;
	std::optional<Token> token{ };

	ParserContext(std::string source, Tokenizers tokenizers)
		: stream(source), tokenizers(tokenizers), tokenize(tokenizers.at(ParserState::Default)){
			this->state.push(ParserState::Default);
		}

	Token& Current() {
		if (this->token == std::nullopt) {
			this->token = this->tokenize(this->stream);
		}

		return *this->token;
	}

	void PushState(ParserState state) {
		this->state.push(state);
		this->tokenize = this->tokenizers.at(this->state.top());
	}

	void PopState() {
		this->state.pop();
		this->tokenize = this->tokenizers.at(this->state.top());
	}

	Token Use() { Token old = this->Current(); this->token = std::nullopt; return old; }
};

void* Parse{}(ParserContext& context);

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

using ProductionSpecifier = std::pair<ProductionType, size_t>;
using WorkItem = std::variant<Symbol, ProductionSpecifier>;
using Argument = std::variant<Token, void*>;

void* Parse{}(ParserContext& context) {
  std::vector<Production> table = GetParseTable();

  std::stack<WorkItem> stack{ };
  std::stack<Argument> args{ };

  stack.push(Symbol{ ProductionType::{} });

  while (!stack.empty()) {
    WorkItem item = stack.top();
    stack.pop();

    if (std::holds_alternative<ProductionSpecifier>(item)) {
      ProductionSpecifier specifier = std::get<ProductionSpecifier>(item);
      Production& production = table[static_cast<uint8_t>(specifier.first)];
      Alternation& alternation = production.alternations[specifier.second];

      // pop this many things off
      std::vector<Argument> arguments{ };
      for (size_t i = 0; i < alternation.symbols.size(); i++) {
        arguments.push_back(args.top());
        args.pop();
      }

      auto result = alternation.code(context, arguments);
      args.push(result);

      continue;
    }

    auto symbol = std::get<Symbol>(item);
    if (std::holds_alternative<TokenType>(symbol)) {
      args.push(context.Use());
      continue;
    }

    ProductionType productionType = std::get<ProductionType>(symbol);
    Production& production = table[static_cast<uint8_t>(productionType)];

    auto first = production.first.find(context.Current().first);
    if (first == production.first.end()) {
      auto result =
        production.alternations[production.alternations.size() - 1].code(
          context, { });
      args.push(result);
      continue;
    }

    stack.push(std::make_pair(productionType, first->second));

    std::vector<Symbol> alternation =
      production.alternations[first->second].symbols;
    for (size_t i = alternation.size(); i-- > 0;) {
      stack.push(alternation[i]);
    }
  }

  return std::get<void*>(args.top());
}

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