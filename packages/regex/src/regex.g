{
#include "AST.h"
}

<regex> {ast::RegularExpressionNode*} ::= <Sequence> <EndOfFile> { return P0; };

<Sequence> {ast::RegularExpressionNode*} ::= <Expression> <Sequence> { return ast::RegularExpressionNode::Add(P0, P1); }
                                     | { return new ast::RegularExpressionNode(); };

<Expression> {ast::ExpressionNode*} ::= <Value> <OptionalQuantifier> { return ast::ExpressionNode::Create(P0, P1); }; 

<Value> {ast::ValueNode*} ::= <Character>      { return new ast::ValueNode(P0.second[0]); }
                            | <CharacterClass> { return ast::ValueNode::Create(P0); };

<CharacterClass> {ast::CharacterClassNode*} ::= <LeftBracket> <CharacterClassPrime> <RightBracket> { return P1; };
<CharacterClassPrime> {ast::CharacterClassNode*}::= <Not> <CharacterClassList> { return ast::CharacterClassNode::Negate(P1); }
                                                  | <CharacterClassList>       { return P0; };

<CharacterClassList> {ast::CharacterClassNode*} ::= <Character> <CharacterClassList> { return ast::CharacterClassNode::Add(P0.second[0], P1); }
                                                        | { return new ast::CharacterClassNode(); };

<OptionalQuantifier> {ast::QuantifierNode*} ::= <Quantifier> { return new ast::QuantifierNode(P0.second); }
                                                  | { return nullptr; };
