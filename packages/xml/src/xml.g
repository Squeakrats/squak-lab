{
#include "ast.h"
}

[
<Default> [
  <> ::= <[\t\n\r ]+>;
  <XMLDesc> ::= <<\\?xml version=\"1.0\" encoding=\"UTF-8\"\\?\x3E>;
  <OPENTAGSTART> ::= <<[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]+>;
  <CLOSETAGSTART> ::= <</[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]+>;
  <TEXT> ::= <[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890().,&#_-/:;\\*\t\n\r ]+>;
  <COMMENT> ::= <<!--[^\n]*--\x3E>;
  <GT> ::= <\x3E>;
]
<InsideTag> [
 <EQ> ::= <=>;
 <NAME> ::= <[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]+[\t\n\r ]*>;
 <STRING> ::= <\"[^\"]*\"[\t\n\r ]*>;
 <SELFCLOSE> ::= </\x3E>;
 <GT> ::= <\x3E>;
 <> ::= <[\t\n\r ]+>;
]
]

<xml> {ast::Document} ::= <XMLDesc> <element> <EndOfFile> { return ast::Document{P1}; };

<attributes> {std::list<ast::Attribute>} ::= <attribute> <attributes> { return ast::CreateAttribute(P0, P1); }
                                           | { return std::list<ast::Attribute>{}; };

<attribute> {ast::Attribute} ::= <NAME> <EQ> <STRING> { return ast::Attribute{P0.second, P2.second}; };

<tagstart> {std::string} ::= <OPENTAGSTART> { context.PushState(ParserState::InsideTag); return P0.second; };
<tagend> {void*} ::= <GT> { context.PopState(); return nullptr; };
<tagselfclose> {void*} ::= <SELFCLOSE> { context.PopState(); return nullptr; };

<element> {std::shared_ptr<ast::Element>} ::= <tagstart> <attributes> <elementPrime> { return std::make_shared<ast::Element>(ast::OpenTag{P0, P1}, P2); };
<elementPrime> {std::list<std::shared_ptr<ast::Node>>} ::= <tagend> <elements> <CLOSETAGSTART> <GT> { return P1; }          
                                                 | <tagselfclose> { return std::list<std::shared_ptr<ast::Node>>{}; };

<elements> {std::list<std::shared_ptr<ast::Node>>} ::= <element> <elements> { return ast::CreateNode(P0, P1); }
                   | <COMMENT> <elements> { return std::list<std::shared_ptr<ast::Node>>{}; }
                   | <TEXT> <elements> { return ast::CreateNode(std::make_shared<ast::TextNode>(P0.second), P1); }
                   | { return std::list<std::shared_ptr<ast::Node>>{}; };
