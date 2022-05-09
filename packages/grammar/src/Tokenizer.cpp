#include "Tokenizer.h"
#include "utility.h"

Token Tokenize(std::stringstream& source) {
  if (source.eof()) {
    return Token{ TokenType::EndOfFile, "" };
  }

  while (true) {
    switch (source.get()) {
      case '<': {
        if (source.peek() == '>') {
          source.get();

          return Token{ TokenType::Symbol, "" };
        }

        std::stringbuf symbol{};
        source.get(symbol, '>');
        Assert(source.get() == '>', "unhandled token");

        return Token{ TokenType::Symbol, symbol.str() };
      }
      case '{': {
        std::stringbuf code{};
        source.get(code, '}');
        Assert(source.get() == '}', "unhandled token");

        return Token{ TokenType::Code, code.str() };
      }
      case ':': {
        Assert(source.get() == ':' && source.get() == '=', "unhandled token");

        return Token{ TokenType::Replaces, "::=" };
      }
      case '|':
        return Token{ TokenType::Alternate, "|" };
      case ';':
        return Token{ TokenType::SemiColon, ";" };
      case '[':
        return Token{ TokenType::LeftBracket, "[" };
      case ']':
        return Token{ TokenType::RightBracket, "]" };
      case '\n':
      case ' ':
      case '\t':
      case '\r':
        break;
      case EOF:
        return Token{ TokenType::EndOfFile, "" };
      default:
        Panic("unexpected token");
    }
  }
}
