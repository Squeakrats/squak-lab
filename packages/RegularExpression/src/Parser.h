// This file was auto-generated 
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

std::shared_ptr<AST::RegularExpression> ParseRegularExpression(ParserContext& context);
std::shared_ptr<AST::Sequence> ParseOptionalSequence(ParserContext& context);
std::shared_ptr<AST::Expression> ParseExpression(ParserContext& context);
std::shared_ptr<AST::Value> ParseValue(ParserContext& context);
std::shared_ptr<AST::CharacterClass> ParseCharacterClass(ParserContext& context);
std::shared_ptr<AST::Not> ParseOptionalNot(ParserContext& context);
std::shared_ptr<AST::CharacterClassList> ParseCharacterClassList(ParserContext& context);
std::shared_ptr<AST::Quantifier> ParseOptionalQuantifier(ParserContext& context);

}
