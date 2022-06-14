{
#include "AST.h"
}

<regex> {ast::RegularExpressionNode*} ::= <Sequence> <EndOfFile> { out = P0; };

<Sequence> {ast::RegularExpressionNode*} ::= <Expression> <Sequence> { out = P1->Prepend(P0); }
                                           | { out = new ast::RegularExpressionNode(); };

<Expression> {ast::ExpressionNode*} ::= <Value> <OptionalQuantifier> { out = new ast::ExpressionNode(P0, P1); }; 

<Value> {ast::ValueNode*} ::= <Character>      { out = new ast::ValueNode(P0.second[0]); }
                            | <CharacterClass> { out = new ast::ValueNode(P0); };

<CharacterClass> {ast::CharacterClassNode*} ::= <LeftBracket> <CharacterClassPrime> <RightBracket> { out = P1; };
<CharacterClassPrime> {ast::CharacterClassNode*}::= <Not> <CharacterClassList> { P1->negated = true; out = P1; }
                                                  | <CharacterClassList>       { out = P0; };

<CharacterClassList> {ast::CharacterClassNode*} ::= <Character> <CharacterClassList> { P1->list += P0.second[0] ; out = P1; }
                                                  | { out = new ast::CharacterClassNode(); };

<OptionalQuantifier> {ast::QuantifierNode*} ::= <Quantifier> { out = new ast::QuantifierNode(P0.second); }
                                              | { out =  nullptr; };
