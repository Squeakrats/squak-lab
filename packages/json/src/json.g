{
#include "AST.h"
}

[
    <LeftBrace> ::= <{>;
    <RightBrace> ::= <}>;
    <Colon> ::= <:>;
    <Comma> ::= <,>;
    <True> ::= <true>;
    <False> ::= <false>;
    <StringLiteral> ::= <\"[^\"]*\">;
    <NumberLiteral> ::= <[0123456789]+>;
    <> ::= <[\t\n ]>;
]

<json> {std::shared_ptr<ast::Object>} ::= <Object> <EndOfFile> { return P0; };

<Object> {std::shared_ptr<ast::Object>} ::= <LeftBrace> <ObjectEntries> <RightBrace> { return std::make_shared<ast::Object>(P1); };

<ObjectEntries> {std::shared_ptr<ast::ObjectEntries>} ::= <ObjectEntry> <OptionalComma> <ObjectEntries> { return std::make_shared<ast::ObjectEntries>(P0, P2); };

<ObjectEntry> {std::shared_ptr<ast::ObjectEntry>} ::= <StringLiteral> <Colon> <Value> { return std::make_shared<ast::ObjectEntry>(P0.second, P2); };

<OptionalComma> {void*} ::= <Comma> { return nullptr; };

<Value> {std::shared_ptr<ast::Value>} ::= <True> { return std::make_shared<ast::Value>(true); }
                  | <False> { return std::make_shared<ast::Value>(false); }
                  | <StringLiteral> { return std::make_shared<ast::Value>(P0.second); }
                  | <NumberLiteral> { return std::make_shared<ast::Value>(std::stod(P0.second)); };