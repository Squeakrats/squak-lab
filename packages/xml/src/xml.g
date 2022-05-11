{
#include "ast.h"
}

[
  <XMLDesc> ::= <<\\?xml version=\"1.0\" encoding=\"UTF-8\"\\?\x3E>;
  <LT> ::= <<>;
  <Slash> ::= </>;
  <GT> ::= <\x3E>;
  <tagname> ::= <[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]+>;
  <> ::= <[\t\n\r ]>;
]

<xml> {ast::Element} ::= <XMLDesc> <element> <EndOfFile> { return ast::Element{}; };

<opentag> {int} ::= <LT> <tagname> <GT> { return 0; };
<closetag> {int} ::= <LT> <Slash> <tagname> <GT> { return 0; };
<element> {ast::Element} ::= <opentag> <closetag> { return ast::Element{}; };
