#include "BNFTokenizer.h"

BNFTokenizer::Token BNFTokenizer::Next() {
	if (this->source.eof()) {
		return Token{ TokenType::EndOfFile, "" };
	}

	while (true) {
		switch (this->source.get()) {
			case '<': {
				std::stringbuf symbol{};
				this->source.get(symbol, '>');
				assert(this->source.get() == '>');

				return Token{ TokenType::Symbol, symbol.str() };
			}
			case '"': {
				std::stringbuf literal{};
				this->source.get(literal, '"');
				assert(this->source.get() == '"');

				return Token{ TokenType::Literal, literal.str() };
			}
			case ':': {
				assert(source.get() == ':' && source.get() == '=');

				return Token{ TokenType::Replaces, "::=" };
			}
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