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

<json> {ast::ObjectNode*} ::= <Object> <EndOfFile> { out = P0; };

<Object> {ast::ObjectNode*} ::= <LeftBrace> <ObjectEntries> <RightBrace> { out = P1; };
<ObjectEntries> {ast::ObjectNode*} ::= <ObjectEntry> <ObjectEntriesPrime> { out = ast::ObjectNode::Add(P0, P1); }
                                     | { out = new ast::ObjectNode(); };

<ObjectEntriesPrime> {ast::ObjectNode*} ::= <Comma> <ObjectEntry> <ObjectEntriesPrime> { out = ast::ObjectNode::Add(P1, P2); }
                                          | { out = new ast::ObjectNode{}; };

<ObjectEntry> {ast::ObjectEntryNode*} ::= <StringLiteral> <Colon> <Value> { out = new ast::ObjectEntryNode{P0.second, P2}; };

<Array> {ast::ArrayNode*} ::= <LeftBracket> <ArrayEntries> <RightBracket> { out = P1; };
<ArrayEntries> {ast::ArrayNode*} ::= <Value> <ArrayEntriesPrime> { out = ast::ArrayNode::Add(P0, P1); }
                                   | { out = new ast::ArrayNode{}; };
<ArrayEntriesPrime> {ast::ArrayNode*} ::= <Comma> <Value> <ArrayEntriesPrime> { out = ast::ArrayNode::Add(P1, P2); }
                                        | { out = new ast::ArrayNode{}; };
                                                        
<Value> {ast::Node*} ::= <True> { out = new ast::BoolNode(true); }
                       | <False> { out = new ast::BoolNode(false); }
                       | <StringLiteral> { out = new ast::StringNode(P0.second); }
                       | <NumberLiteral> { out = new ast::DoubleNode(std::stod(P0.second)); }
                       | <Object> { out = P0; }
                       | <Array> { out = P0; };
