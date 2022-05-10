{
#include "AST.h"
}

[
    <LeftBrace> ::= <{>;
    <RightBrace> ::= <}>;
    <LeftBracket> ::= <\\[>;
    <RightBracket> ::= <\\]>;
    <Colon> ::= <:>;
    <Comma> ::= <,>;
    <True> ::= <true>;
    <False> ::= <false>;
    <StringLiteral> ::= <\"[^\"]*\">;
    <NumberLiteral> ::= <[-e.0123456789]+>;
    <> ::= <[\t\n\r ]>;
]

<json> {std::shared_ptr<ast::Object>} ::= <Object> <EndOfFile> { return P0; };

<Object> {std::shared_ptr<ast::Object>} ::= <LeftBrace> <ObjectEntries> <RightBrace> { return std::make_shared<ast::Object>(P1); };
<ObjectEntries> {std::shared_ptr<ast::ObjectEntries>} ::= <ObjectEntry> <ObjectEntriesPrime> { return std::make_shared<ast::ObjectEntries>(P0, P1); }
                                                        | { return nullptr; };

<ObjectEntriesPrime> {std::shared_ptr<ast::ObjectEntries>} ::= <Comma> <ObjectEntry> <ObjectEntriesPrime> { return std::make_shared<ast::ObjectEntries>(P1, P2); }
                                                             | { return nullptr; };

<ObjectEntry> {std::shared_ptr<ast::ObjectEntry>} ::= <StringLiteral> <Colon> <Value> { return std::make_shared<ast::ObjectEntry>(P0.second, P2); };

<Array> {std::shared_ptr<ast::Array>} ::= <LeftBracket> <ArrayEntries> <RightBracket> { return std::make_shared<ast::Array>(P1); };
<ArrayEntries> {std::shared_ptr<ast::ArrayEntry>} ::= <Value> <ArrayEntriesPrime> { return std::make_shared<ast::ArrayEntry>(P0, P1); }
                                                    | { return nullptr; };
<ArrayEntriesPrime> {std::shared_ptr<ast::ArrayEntry>} ::= <Comma> <Value> <ArrayEntriesPrime> { return std::make_shared<ast::ArrayEntry>(P1, P2); }
                                                         | { return nullptr; };
                                                        
<Value> {std::shared_ptr<ast::Value>} ::= <True> { return std::make_shared<ast::Value>(true); }
                  | <False> { return std::make_shared<ast::Value>(false); }
                  | <StringLiteral> { return std::make_shared<ast::Value>(P0.second); }
                  | <NumberLiteral> { return std::make_shared<ast::Value>(std::stod(P0.second)); }
                  | <Object> { return std::make_shared<ast::Value>(P0); }
                  | <Array> { return std::make_shared<ast::Value>(P0); };
