// This file was auto-generated 
#pragma once
#include "TokenStream.h"

#include "AST.h"

namespace json {

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

std::shared_ptr<ast::Object> Parsejson(ParserContext& context);
std::shared_ptr<ast::Object> ParseObject(ParserContext& context);
std::shared_ptr<ast::ObjectEntries> ParseObjectEntries(ParserContext& context);
std::shared_ptr<ast::ObjectEntry> ParseObjectEntry(ParserContext& context);
void* ParseOptionalComma(ParserContext& context);
std::shared_ptr<ast::Value> ParseValue(ParserContext& context);

}
