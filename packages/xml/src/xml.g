{
#include "ast.h"
}

[
  <XMLDesc> ::= <<\\?xml version=\"1.0\" encoding=\"UTF-8\"\\?\x3E>;
  <> ::= <[\t\n\r ]>;
]

<xml> {ast::Element} ::= <XMLDesc> <EndOfFile> { return ast::Element{}; };
