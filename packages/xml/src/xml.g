{
#include "ast.h"
}

[
  <> ::= <[\t\n\r ]+>;
  <XMLDesc> ::= <<\\?xml version=\"1.0\" encoding=\"UTF-8\"\\?\x3E>;
  <LT> ::= <<>;
  <GT> ::= <\x3E>;
  <closetagstart> ::= <</>;
  <tagname> ::= <[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]+>;
  <text> ::= <[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123457890().,-/:\t\n\r ]+>;
]

<xml> {ast::Element} ::= <XMLDesc> <element> <EndOfFile> { return ast::Element{}; };

<opentag> {int} ::= <LT> <tagname> <GT> { return 0; };
<closetag> {int} ::= <closetagstart> <tagname> <GT> { return 0; };
<element> {ast::Element} ::= <opentag> <innerElement> <closetag> { return ast::Element{}; };
<innerElement> {int} ::= <element> { return 0; }
                       | <text> { return 0; }
                       | { return 0; };
