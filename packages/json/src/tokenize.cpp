#include "parser.h"
#include "RegularExpression.h"

namespace JSON {

Token Tokenize(std::stringstream& stream) {
	static std::vector<TokenType> categories = {
		TokenType::LeftBrace,
		TokenType::RightBrace,
		TokenType::StringLiteral,
		TokenType::Colon,
	};
	static DFA dfa = DFA::FromNFA(RegularExpression::Create(std::vector<std::string>({
		"{",
		"}",
		"\"[^\"]*\"",
		":",
	})));

	auto longest = dfa.Longest(stream);
	if (longest.second != 0 && longest.second <= categories.size()) {
		return std::make_pair(categories[longest.second - 1], longest.first);
	}
	
	return std::make_pair(TokenType::EndOfFile, "");
}

};