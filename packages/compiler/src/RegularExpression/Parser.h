#pragma once
#include "TokenStream.h"

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
        Token Use() { 
            Token old = this->token;
            this->token = this->stream.Next();
            return old;
        }
    };

    void* ParseOptionalQuantifier(ParserContext& context);
    void* ParseExpression(ParserContext& context);
    void* ParseOptionalSequence(ParserContext& context);
    void* ParseRegularExpression(ParserContext& context);

}