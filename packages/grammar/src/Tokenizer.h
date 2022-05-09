#pragma once
#include <assert.h>
#include <sstream>

enum class TokenType {
  Symbol,
  Replaces,
  SemiColon,
  Code,
  LeftBracket,
  RightBracket,
  Alternate,
  EndOfFile
};

using Token = std::pair<TokenType, std::string>;

Token Tokenize(std::stringstream& source);
