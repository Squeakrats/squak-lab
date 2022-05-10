{
#include "AST.h"
}

<regex> {AST::RegularExpression} ::= <Sequence> <EndOfFile> { return AST::RegularExpression{P0}; };

<Sequence> {AST::Sequence} ::= <Expression> <Sequence> { return AST::CreateSequence(P0, P1); }
                                              | { return std::vector<AST::Expression>(); };

<Expression> {AST::Expression} ::= <Value> <OptionalQuantifier> { return AST::Expression{P0, P1}; }; 

<Value> {AST::Value} ::= <Character> { return P0.second[0]; }
                        | <CharacterClass> { return P0; };

<CharacterClass> {AST::CharacterClass} ::= <LeftBracket> <OptionalNot> <CharacterClassList> <RightBracket> { return AST::CharacterClass{P1, P2}; };

<OptionalNot> {bool} ::= <Not> { return true; }
                       | { return false; };

<CharacterClassList> {std::string} ::= <Character> <CharacterClassList> { return P0.second[0] + P1; }
                                                                  | { return ""; };

<OptionalQuantifier> {std::optional<char>} ::= <Quantifier> { return P0.second[0]; }
                                                  | { return std::nullopt; };
