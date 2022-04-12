#include <assert.h>
#include "Parser.generated.h"

namespace RegularExpression {

Token Tokenize(std::stringstream& source) {
	if (source.eof()) {
		return Token{ TokenType::EndOfFile, "" };
	}

	while (true) {
		char current;
		switch (current = source.get()) {
			case '+':
			case '*':
			case '?':
				return Token{ TokenType::Quantifier, std::string(1, current) };
			case '^':
				return Token{ TokenType::Not, std::string(1, current) };
			case '[':
				return Token{ TokenType::LeftBracket, std::string(1, current) };
			case ']':
				return Token{ TokenType::RightBracket, std::string(1, current) };
			case '\\':
			{
				switch (current = source.get()) {
					case '[':
					case ']':
						return Token{ TokenType::Character, std::string(1, current) };
					default:
						throw std::exception();
				}	
			}
				
			case EOF:
				return Token{ TokenType::EndOfFile, "" };
			default:
				return Token{ TokenType::Character, std::string(1, current) };
		}
	}
}

}
