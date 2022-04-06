#pragma once
#include "TokenStream.h"

#include "AST.h"

namespace RegularExpression {

enum class TokenType {
	Character,
	EndOfFile,
	Quantifier,
};

using Token = std::pair<TokenType, std::string>;

Token Tokenize(std::stringstream& source);

class ParserContext {
public:
	Token token;
	TokenStream<Token>& stream;
	Token Use() { auto old = this->token; this->token = this->stream.Next(); return old; }
};

AST::RegularExpression* ParseRegularExpression(ParserContext& context);
AST::Sequence* ParseOptionalSequence(ParserContext& context);
AST::Expression* ParseExpression(ParserContext& context);
AST::Quantifier* ParseOptionalQuantifier(ParserContext& context);

}
