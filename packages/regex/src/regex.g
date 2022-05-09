{
#include "AST.h"
}

<regex> {std::shared_ptr<AST::RegularExpression>} ::= <OptionalSequence> <EndOfFile> { return std::make_shared<AST::RegularExpression>(P0); }; 

<OptionalSequence> {std::shared_ptr<AST::Sequence>} ::= <Expression> <OptionalSequence> { return std::make_shared<AST::Sequence>(P0, P1); }; 

<Expression> {std::shared_ptr<AST::Expression>} ::= <Value> <OptionalQuantifier> { return std::make_shared<AST::Expression>(P0, P1); }; 

<Value> {std::shared_ptr<AST::Value>} ::= <Character> { return std::make_shared<AST::Value>(std::make_shared<AST::Character>(P0.second)); }
                        | <CharacterClass> { return std::make_shared<AST::Value>(P0); };

<CharacterClass> {std::shared_ptr<AST::CharacterClass>} ::= <LeftBracket> <OptionalNot> <CharacterClassList> <RightBracket> { return std::make_shared<AST::CharacterClass>(P1 != nullptr, P2); };

<OptionalNot> {std::shared_ptr<AST::Not>} ::= <Not> { return std::make_shared<AST::Not>(); };

<CharacterClassList> {std::shared_ptr<AST::CharacterClassList>} ::= <Character> <CharacterClassList> { return std::make_shared<AST::CharacterClassList>(P0.second, P1); };

<OptionalQuantifier> {std::shared_ptr<AST::Quantifier>} ::= <Quantifier> { return std::make_shared<AST::Quantifier>(P0.second); }; 
