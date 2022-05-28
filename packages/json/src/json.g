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

<json> {std::shared_ptr<ast::Object>} ::= <Object> <EndOfFile> { out = P0; };

<Object> {std::shared_ptr<ast::Object>} ::= <LeftBrace> <ObjectEntries> <RightBrace> { out = std::make_shared<ast::Object>(ast::Object{ P1 }); };
<ObjectEntries> {std::list<ast::ObjectEntry>} ::= <ObjectEntry> <ObjectEntriesPrime> { out = ast::Object::Create(P0, P1); }
                                                        | { out = std::list<ast::ObjectEntry>(); };

<ObjectEntriesPrime> {std::list<ast::ObjectEntry>} ::= <Comma> <ObjectEntry> <ObjectEntriesPrime> { out = ast::Object::Create(P1, P2); }
                                                             | { out = std::list<ast::ObjectEntry>(); };

<ObjectEntry> {ast::ObjectEntry} ::= <StringLiteral> <Colon> <Value> { out = ast::ObjectEntry{P0.second, P2}; };

<Array> {std::shared_ptr<ast::Array>} ::= <LeftBracket> <ArrayEntries> <RightBracket> { out = std::make_shared<ast::Array>(P1); };
<ArrayEntries> {ast::Array} ::= <Value> <ArrayEntriesPrime> { out = ast::Array::Create(P0, P1); }
                                                    | { out = ast::Array{}; };
<ArrayEntriesPrime> {ast::Array} ::= <Comma> <Value> <ArrayEntriesPrime> { out = ast::Array::Create(P1, P2); }
                                                         | { out = ast::Array{}; };
                                                        
<Value> {ast::Value} ::= <True> { out = true; }
                  | <False> { out = false; }
                  | <StringLiteral> { out = P0.second; }
                  | <NumberLiteral> { out = std::stod(P0.second); }
                  | <Object> { out =  P0; }
                  | <Array> { out = P0; };
