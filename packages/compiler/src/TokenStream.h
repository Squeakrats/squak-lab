#pragma once
#include <functional>
#include <sstream>

template<typename Token>
class TokenStream {
  using Tokenizer = std::function<Token(std::stringstream&)>;

private:
  std::stringstream source;
  Tokenizer tokenizer;

public:
  TokenStream(std::string source, Tokenizer tokenizer)
    : source(source)
    , tokenizer(tokenizer){};

  Token Next() { return this->tokenizer(this->source); }

  bool EndOfStream() { return this->source.eof(); }
};