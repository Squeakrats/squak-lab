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
  <COMMENTSTART> ::= <<!-->;
  <GT> ::= <\x3E>;
]
<InsideTag> [
 <EQ> ::= <=>;
 <NAME> ::= <[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]+>;
 <STRING> ::= <\"[^\"]*\">;
 <SELFCLOSE> ::= </\x3E>;
 <GT> ::= <\x3E>;
 <> ::= <[\t\n\r ]+>;
]
<InsideComment> [
 <COMMENTTEXT> ::= <[^-]+>;
 <COMMENTDASH> ::= <->;
 <COMMENTEND> ::= <--\x3E>;
]
]

<xml> {ast::Document} ::= <XMLDesc> <element> <EndOfFile> { out = ast::Document{P1}; };

<attributes> {std::list<ast::Attribute>} ::= <attribute> <attributes> { out = ast::CreateAttribute(P0, P1); }
                                           | { out = std::list<ast::Attribute>{}; };

<attribute> {ast::Attribute} ::= <NAME> <EQ> <STRING> { out = ast::Attribute{P0.second, P2.second}; };

<tagstart> {std::string} ::= <OPENTAGSTART> { context.PushState(ParserState::InsideTag); out = P0.second; };
<tagend> {void*} ::= <GT> { context.PopState(); out = nullptr; };
<tagselfclose> {void*} ::= <SELFCLOSE> { context.PopState(); out = nullptr; };

<element> {std::shared_ptr<ast::Element>} ::= <tagstart> <attributes> <elementPrime> { out = std::make_shared<ast::Element>(ast::OpenTag{P0, P1}, P2); };
<elementPrime> {std::list<std::shared_ptr<ast::Node>>} ::= <tagend> <elements> <CLOSETAGSTART> <GT> { out = P1; }          
                                                 | <tagselfclose> { out = std::list<std::shared_ptr<ast::Node>>{}; };

<commentstart> {void*} ::= <COMMENTSTART> { context.PushState(ParserState::InsideComment); out = nullptr; };
<commentPrime> {void*} ::= <COMMENTTEXT> <commentPrime> { out = nullptr; }
                         | <COMMENTDASH> <commentPrime> { out = nullptr; }
                         | <COMMENTEND> { out = nullptr; };

<comment> {void*} ::= <commentstart> <commentPrime> { context.PopState(); out = nullptr; }
                    | { out = nullptr;};

<elements> {std::list<std::shared_ptr<ast::Node>>} ::= <element> <elements> { out = ast::CreateNode(P0, P1); }
                   | <comment> <elements> { out = std::list<std::shared_ptr<ast::Node>>{}; }
                   | <TEXT> <elements> { out = ast::CreateNode(std::make_shared<ast::TextNode>(P0.second), P1); }
                   | { out = std::list<std::shared_ptr<ast::Node>>{}; };
