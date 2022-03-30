#include "BNF.h"

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
				std::stringbuf terminal{};
				this->source.get(terminal, '"');
				assert(this->source.get() == '"');

				return Token{ TokenType::Terminal, terminal.str() };
			}
			case ':': {
				assert(source.get() == ':' && source.get() == '=');

				return Token{ TokenType::Replaces, "::=" };
			}
			case '\n':
				return Token{ TokenType::NewLine, "\n" };
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