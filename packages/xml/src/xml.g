{
#include "ast.h"
}

[
  <> ::= <[\t\n\r ]+>;
  <XMLDesc> ::= <<\\?xml version=\"1.0\" encoding=\"UTF-8\"\\?\x3E>;
  <OPENTAGSTART> ::= <<[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]+[\t\n\r ]*>;
  <CLOSETAGSTART> ::= <</[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]+[\t\n\r ]*>;
  <GT> ::= <\x3E>;
  <EQ> ::= <=>;
  <NAME> ::= <[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]+>;
  <STRING> ::= <\"[^\"]*\">;
  <TEXT> ::= <[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890().,&#_-/:;\t\n\r ]+>;
  <COMMENT> ::= <<!--[^\n]*--\x3E>;
]

<xml> {ast::Document} ::= <XMLDesc> <element> <EndOfFile> { return ast::Document{P1}; };

<opentag> {ast::OpenTag} ::= <OPENTAGSTART> <attributes> <GT> { return ast::OpenTag{P0.second, P1}; };
<attributes> {std::list<ast::Attribute>} ::= <attribute> <attributes> { return ast::CreateAttribute(P0, P1); }
                                           | { return std::list<ast::Attribute>{}; };

<attribute> {ast::Attribute} ::= <NAME> <EQ> <STRING> { return ast::Attribute{P0.second, P2.second}; };

<element> {std::shared_ptr<ast::Element>} ::= <opentag> <elements> <CLOSETAGSTART> <GT> { return std::make_shared<ast::Element>(P0, P1); };
<elements> {std::list<std::shared_ptr<ast::Node>>} ::= <element> <elements> { return ast::CreateNode(P0, P1); }
                   | <COMMENT> <elements> { return std::list<std::shared_ptr<ast::Node>>{}; }
                   | <text> <elements> { return ast::CreateNode(std::make_shared<ast::TextNode>(P0), P1); }
                   | { return std::list<std::shared_ptr<ast::Node>>{}; };

<text> {std::string} ::= <NAME> { return P0.second; }
               | <TEXT> { return P0.second; };