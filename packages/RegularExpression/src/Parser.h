#pragma once
#include "TokenStream.h"

#include "AST.h"

namespace RegularExpression {

enum class TokenType {
	Character,
	EndOfFile,
	LeftBracket,
	Not,
	Quantifier,
	RightBracket,
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
AST::Value* ParseValue(ParserContext& context);
AST::CharacterClass* ParseCharacterClass(ParserContext& context);
AST::Not* ParseOptionalNot(ParserContext& context);
AST::CharacterClassList* ParseCharacterClassList(ParserContext& context);
AST::Quantifier* ParseOptionalQuantifier(ParserContext& context);

}
