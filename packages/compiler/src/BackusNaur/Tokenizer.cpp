#include "Tokenizer.h"

namespace BackusNaur {

Token Tokenize(std::stringstream& source) {
	if (source.eof()) {
		return Token{ TokenType::EndOfFile, "" };
	}

	while (true) {
		switch (source.get()) {
		case '<': {
			std::stringbuf symbol{};
			source.get(symbol, '>');
			assert(source.get() == '>');

			return Token{ TokenType::Symbol, symbol.str() };
		}
		case ':': {
			assert(source.get() == ':' && source.get() == '=');

			return Token{ TokenType::Replaces, "::=" };
		}
		case '|':
			return Token{ TokenType::Alternate, "|" };
		case ';':
			return Token{ TokenType::SemiColon, ";" };
		case '\n':
		case ' ':
		case '\t':
			break;
		case EOF:
			return Token{ TokenType::EndOfFile, "" };
		default:
			assert(false);
		}
	}
}

}
