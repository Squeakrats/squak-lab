{
#include "AST.h"
}

<regex> {ast::RegularExpressionNode*} ::= <Sequence> <EndOfFile> { out = P0; };

<Sequence> {ast::RegularExpressionNode*} ::= <Expression> <Sequence> { out = ast::RegularExpressionNode::Add(P0, P1); }
                                     | { out = new ast::RegularExpressionNode(); };

<Expression> {ast::ExpressionNode*} ::= <Value> <OptionalQuantifier> { out = ast::ExpressionNode::Create(P0, P1); }; 

<Value> {ast::ValueNode*} ::= <Character>      { out = new ast::ValueNode(P0.second[0]); }
                            | <CharacterClass> { out = ast::ValueNode::Create(P0); };

<CharacterClass> {ast::CharacterClassNode*} ::= <LeftBracket> <CharacterClassPrime> <RightBracket> { out = P1; };
<CharacterClassPrime> {ast::CharacterClassNode*}::= <Not> <CharacterClassList> { out = ast::CharacterClassNode::Negate(P1); }
                                                  | <CharacterClassList>       { out = P0; };

<CharacterClassList> {ast::CharacterClassNode*} ::= <Character> <CharacterClassList> { out = ast::CharacterClassNode::Add(P0.second[0], P1); }
                                                        | { out = new ast::CharacterClassNode(); };

<OptionalQuantifier> {ast::QuantifierNode*} ::= <Quantifier> { out = new ast::QuantifierNode(P0.second); }
                                                  | { out =  nullptr; };
