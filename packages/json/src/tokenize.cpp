#include "parser.h"

namespace JSON {

Token Tokenize(std::stringstream& stream) {
	return std::make_pair(TokenType::EndOfFile, "");
}

};