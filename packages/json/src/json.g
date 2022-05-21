{
#include "AST.h"
}

[
<Default> [
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
]

<json> {std::shared_ptr<ast::Object>} ::= <Object> <EndOfFile> { return P0; };

<Object> {std::shared_ptr<ast::Object>} ::= <LeftBrace> <ObjectEntries> <RightBrace> { return std::make_shared<ast::Object>(ast::Object{ P1 }); };
<ObjectEntries> {std::list<ast::ObjectEntry>} ::= <ObjectEntry> <ObjectEntriesPrime> { return ast::Object::Create(P0, P1); }
                                                        | { return std::list<ast::ObjectEntry>(); };

<ObjectEntriesPrime> {std::list<ast::ObjectEntry>} ::= <Comma> <ObjectEntry> <ObjectEntriesPrime> { return ast::Object::Create(P1, P2); }
                                                             | { return std::list<ast::ObjectEntry>(); };

<ObjectEntry> {ast::ObjectEntry} ::= <StringLiteral> <Colon> <Value> { return ast::ObjectEntry{P0.second, P2}; };

<Array> {std::shared_ptr<ast::Array>} ::= <LeftBracket> <ArrayEntries> <RightBracket> { return std::make_shared<ast::Array>(P1); };
<ArrayEntries> {ast::Array} ::= <Value> <ArrayEntriesPrime> { return ast::Array::Create(P0, P1); }
                                                    | { return ast::Array{}; };
<ArrayEntriesPrime> {ast::Array} ::= <Comma> <Value> <ArrayEntriesPrime> { return ast::Array::Create(P1, P2); }
                                                         | { return ast::Array{}; };
                                                        
<Value> {ast::Value} ::= <True> { return true; }
                  | <False> { return false; }
                  | <StringLiteral> { return P0.second; }
                  | <NumberLiteral> { return std::stod(P0.second); }
                  | <Object> { return P0; }
                  | <Array> { return P0; };
