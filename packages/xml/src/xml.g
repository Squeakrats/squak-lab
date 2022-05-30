{
#include "ast.h"
}

[
<Default> [
  <XMLDesc> ::= <<\\?xml version=\"1.0\" encoding=\"UTF-8\"\\?\x3E[\t\n\r ]*>;
  <CLOSETAGSTART> ::= <</[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]+>;
  <TEXT> ::= <[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890().,&#_-/:;\\*\t\n\r ]+>;
  <COMMENTSTART> ::= <<!-->;
  <LT> ::= <<>;
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

<xml> {ast::DocumentNode*} ::= <XMLDesc> <element> <EndOfFile> { out = new ast::DocumentNode{P1}; };

<attributes> {ast::AttributesNode*} ::= <NAME> <EQ> <STRING> <attributes> { out = ast::AttributesNode::Add({P0.second, P2.second}, P3); }
                                      | { out = new ast::AttributesNode{}; };

<tagstart> {void*} ::= <LT> { context.PushState(ParserState::InsideTag); };
<tagend> {void*} ::= <GT> { context.PopState(); };
<tagselfclose> {void*} ::= <SELFCLOSE> { context.PopState(); };

<element> {ast::ElementNode*} ::= <tagstart> <NAME> <attributes> <elementPrime> { out = new ast::ElementNode{P1.second, P2, P3}; };
<elementPrime> {ast::ElementsNode*} ::= <tagend> <elements> <CLOSETAGSTART> <GT> { out = P1; }          
                                      | <tagselfclose> { };

<commentstart> {void*} ::= <COMMENTSTART> { context.PushState(ParserState::InsideComment); };
<commentPrime> {void*} ::= <COMMENTTEXT> <commentPrime> { }
                         | <COMMENTDASH> <commentPrime> { }
                         | <COMMENTEND> { };

<comment> {void*} ::= <commentstart> <commentPrime> { context.PopState(); }
                    | { };

<elements> {ast::ElementsNode*} ::= <element> <elements> { out = ast::ElementsNode::Add(P0, P1); }
                                  | <comment> <elements> { out = P1; }
                                  | <TEXT> <elements> { out = ast::ElementsNode::Add(P0.second, P1); }
                                  | { out = new ast::ElementsNode{}; };
