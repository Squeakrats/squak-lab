#pragma once
#include "TokenStream.h"

#include "AST.h"

namespace JSON {

enum class TokenType {
	Colon,
	Comma,
	EndOfFile,
	False,
	LeftBrace,
	NumberLiteral,
	RightBrace,
	StringLiteral,
	True,
};

using Token = std::pair<TokenType, std::string>;

Token Tokenize(std::stringstream& source);

class ParserContext {
public:
	Token token;
	TokenStream<Token>& stream;
	Token Use() { auto old = this->token; this->token = this->stream.Next(); return old; }
};

void* ParseJSON(ParserContext& context);
void* ParseObject(ParserContext& context);
void* ParseObjectEntries(ParserContext& context);
void* ParseObjectEntry(ParserContext& context);
void* ParseOptionalComma(ParserContext& context);
void* ParseValue(ParserContext& context);

}
