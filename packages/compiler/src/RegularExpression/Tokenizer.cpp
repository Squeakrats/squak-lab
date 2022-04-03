#include <assert.h>
#include "Tokenizer.h"

namespace RegularExpression {

Token Tokenize(std::stringstream& source) {
	if (source.eof()) {
		return Token{ TokenType::EndOfFile, "" };
	}

	while (true) {
		char current;
		switch (current = source.get()) {
			case '+':
				return Token{ TokenType::Quantifier, "+" };
			case '-':
				return Token{ TokenType::Quantifier, "-" };
			case '[':
				return Token{ TokenType::LeftBracket, "[" };
			case ']':
				return Token{ TokenType::RightBracket, "]" };
			case '(':
				return Token{ TokenType::LeftParenthesis, "(" };
			case ')':
				return Token{ TokenType::RightParenthesis, ")" };
			case EOF:
				return Token{ TokenType::EndOfFile, "" };
			default:
				return Token{ TokenType::Character, std::string(1, current) };
		}
	}
}

}
