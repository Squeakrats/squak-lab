#include "Grammar.h"
#include "fragments.h"
#include <fstream>
#include <iostream>
#include <optional>
#ifdef EMSCRIPTEN
#include <emscripten.h>

EM_JS(char*, readFileSync, (const char* path), {
  var contents = fs.readFileSync(UTF8ToString(path)).toString();
  var length = lengthBytesUTF8(contents);

  var buffer = _malloc(length);
  stringToUTF8(contents, buffer, length);

  return buffer;
});

EM_JS(void, writeFileSync, (const char* path, const char* contents), {
  fs.writeFileSync(UTF8ToString(path), UTF8ToString(contents));
});

#endif

std::string ReadFile(std::string path) {
#ifdef EMSCRIPTEN
  char* buffer = readFileSync(path.c_str());
  std::string result(buffer);
  delete buffer;

  return result;
#else
  std::ifstream file(path);
  Assert(file.is_open(), "invalid file");

  std::stringstream stream{};
  stream << file.rdbuf();

  return stream.str();
#endif
}

void EmitFile(std::string path, std::string contents) {
#ifdef EMSCRIPTEN
  writeFileSync(path.c_str(), contents.c_str());
#else
  std::ofstream file(path);
  Assert(file.is_open(), "invalid file");

  file << contents;
  file.close();
#endif
}

std::string GenHeader(Grammar& grammar) {
  std::stringstream tokens{};
  for (auto terminal : grammar.Terminals()) {
    tokens << '\t' << terminal << ",\n";
  }

  std::stringstream declarations{};
  for (auto production : grammar.ast.productions) {
    declarations << production.type << " Parse" << production.symbol
                 << "(ParserContext& context);\n";
  }

  return fragments::format(fragments::Header,
                           { grammar.ast.code,
                             grammar.ast.productions[0].symbol,
                             tokens.str(),
                             "",
                             declarations.str() });
}

std::string GenParser(Grammar& grammar) {
  auto terminals = grammar.Terminals();

  std::string tokenize{};
  for (auto& tokens : grammar.ast.tokens) {
    std::stringstream types{};
    std::stringstream expressions{};
    for (auto token : tokens.second) {
      if (token.first != "") {
        types << "\t\t TokenType::" << token.first << ",\n";
      } else {
        types << "\t\t std::nullopt,\n";
      }

      expressions << "\t\t\"" << token.second << "\",\n";
    }

    tokenize += fragments::format(
      fragments::Tokenize, { tokens.first, types.str(), expressions.str() });
  }

  if (grammar.ast.tokens.size()) {
    std::stringstream entries{};
    for (auto& token : grammar.ast.tokens) {
      entries << "{ ParserState::" + token.first + ", Tokenize" + token.first +
                   " } ,\n";
    }

    tokenize += fragments::format(fragments::GetTokenizers, { entries.str() });
  }

  std::stringstream parsers{};
  for (auto production : grammar.ast.productions) {
    auto rules = grammar.Rules(production.symbol);

    std::stringstream body{};
    std::optional<size_t> epsilon{};
    for (auto rule : rules) {
      if (rule.second.size() == 0) {
        epsilon = rule.first;
        continue;
      }

      for (auto symbol : rule.second) {
        body << "\t\tcase TokenType::" << symbol << ":\n";
      }

      size_t paramaters = 0;
      body << "\t\t{\n";
      for (auto symbol : production.expression.at(rule.first).symbols) {
        if (terminals.find(symbol) != terminals.end()) {
          body << "\t\t\tassert(context.token.first == TokenType::" << symbol
               << ");\n";
          body << "\t\t\tauto P" << paramaters++ << " = context.Use();\n";
        } else {
          body << "\t\t\tauto P" << paramaters++ << " = Parse" << symbol
               << "(context); \n";
        }
      }
      body << "\n\t\t\t{" << production.expression.at(rule.first).code << "}\n";
      body << "\t\t}\n";
    }

    std::string defaultCase = "\t\t\t";
    if (epsilon != std::nullopt) {
      defaultCase += production.expression.at(*epsilon).code;
    } else {
      defaultCase += "std::abort();";
    }

    parsers << fragments::format(
      fragments::ParserImplementation,
      { production.type, production.symbol, body.str(), defaultCase });
  }

  return fragments::format(
    fragments::Parser,
    { grammar.ast.tokens.size() > 0 ? fragments::TokenizerIncludes : "",
      grammar.ast.productions[0].symbol,
      tokenize,
      parsers.str() });
}

int main(int argc, char* argv[]) {
  std::string source = argv[1];
  std::string outDir = argv[2];

  Grammar grammar = Grammar::Create(ReadFile(source));

  EmitFile(outDir + "Parser.generated.h", GenHeader(grammar));
  EmitFile(outDir + "Parser.generated.cpp", GenParser(grammar));

  return 0;
}