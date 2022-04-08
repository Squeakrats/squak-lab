// This file was auto-generated 
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

std::shared_ptr<AST::Object> ParseJSON(ParserContext& context);
std::shared_ptr<AST::Object> ParseObject(ParserContext& context);
std::shared_ptr<AST::ObjectEntries> ParseObjectEntries(ParserContext& context);
std::shared_ptr<AST::ObjectEntry> ParseObjectEntry(ParserContext& context);
void* ParseOptionalComma(ParserContext& context);
std::shared_ptr<AST::Value> ParseValue(ParserContext& context);

}
