{
#include "ast.h"
}

[
<Default> [
  <> ::= <[\t\n\r ]+>;
  <XMLDesc> ::= <<\\?xml version=\"1.0\" encoding=\"UTF-8\"\\?\x3E>;
  <OPENTAGSTART> ::= <<[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]+[\t\n\r ]*>;
  <CLOSETAGSTART> ::= <</[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]+[\t\n\r ]*>;
  <GT> ::= <\x3E>;
  <SELFCLOSE> ::= </\x3E>;
  <EQ> ::= <=>;
  <NAME> ::= <[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]+[\t\n\r ]*>;
  <STRING> ::= <\"[^\"]*\"[\t\n\r ]*>;
  <TEXT> ::= <[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890().,&#_-/:;\\*\t\n\r ]+>;
  <COMMENT> ::= <<!--[^\n]*--\x3E>;
]
]

<xml> {ast::Document} ::= <XMLDesc> <element> <EndOfFile> { return ast::Document{P1}; };

<attributes> {std::list<ast::Attribute>} ::= <attribute> <attributes> { return ast::CreateAttribute(P0, P1); }
                                           | { return std::list<ast::Attribute>{}; };

<attribute> {ast::Attribute} ::= <NAME> <EQ> <STRING> { return ast::Attribute{P0.second, P2.second}; };

<element> {std::shared_ptr<ast::Element>} ::= <OPENTAGSTART> <attributes> <elementPrime> { return std::make_shared<ast::Element>(ast::OpenTag{P0.second, P1}, P2); };
<elementPrime> {std::list<std::shared_ptr<ast::Node>>} ::= <GT> <elements> <CLOSETAGSTART> <GT> { return P1; }          
                                                 | <SELFCLOSE> { return std::list<std::shared_ptr<ast::Node>>{}; };

<elements> {std::list<std::shared_ptr<ast::Node>>} ::= <element> <elements> { return ast::CreateNode(P0, P1); }
                   | <COMMENT> <elements> { return std::list<std::shared_ptr<ast::Node>>{}; }
                   | <text> <elements> { return ast::CreateNode(std::make_shared<ast::TextNode>(P0), P1); }
                   | { return std::list<std::shared_ptr<ast::Node>>{}; };

<text> {std::string} ::= <NAME> { return P0.second; }
               | <TEXT> { return P0.second; };