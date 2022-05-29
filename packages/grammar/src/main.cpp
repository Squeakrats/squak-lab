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

  std::stringstream productions{};
  for (auto production : grammar.ast.productions) {
    productions << '\t' << production.symbol << ",\n";
  }

  std::stringstream parserStates{};
  for (auto token : grammar.ast.tokens) {
    if (token.first != "Default") {
      parserStates << "\t" + token.first + ",\n";
    }
  }

  return fragments::format(fragments::Header,
                           { grammar.ast.code,
                             grammar.ast.productions[0].symbol,
                             tokens.str(),
                             productions.str(),
                             parserStates.str(),
                             grammar.ast.productions[0].symbol
                           });
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

  std::stringstream parseTable{};
  parseTable << "\nstd::vector<Production> GetParseTable()  {\n return {\n";
  for (auto production : grammar.ast.productions) {
    const auto& rules = grammar.Rules(production.symbol);

    parseTable << "{"
               << " /*" << production.symbol << "*/\n";

    parseTable << "\t{\n";
    for (auto rule : rules) {
      for (auto token : rule.second) {
        parseTable << "\t\t{ TokenType::" << token << ", " << rule.first
                   << " },\n";
      }
    }
    parseTable << "\t},\n";

    parseTable << "\t{\n";
    for (auto rule : rules) {
      parseTable << "\t\t{\n";
      parseTable << "\t\t{ ";
      for (auto symbol : production.expression[rule.first].symbols) {
        if (terminals.find(symbol) != terminals.end()) {
          parseTable << "TokenType::" << symbol << ", ";
        } else {
          parseTable << "ProductionType::" << symbol << ", ";
        }
      }
      parseTable << " },\n";
      parseTable << "\t\t[](ParserContext& context, std::vector<std::variant<Token, void*>> arguments) {\n";
      parseTable << "\t\t\tvoid* out{};\n";
      size_t n = 0;
      size_t count =
        production.expression[rule.first]
          .symbols.size();

      for (auto symbol : production.expression[rule.first].symbols) {
        parseTable << "\t\t\tauto P" << n << " = ";
        if (terminals.find(symbol) != terminals.end()) {
          parseTable << "std::get<Token>(arguments[" << count - 1 - n << "]);\n";
        } else {
          parseTable << "static_cast<" << grammar.productions.at(symbol).type << ">(std::get<void*>(arguments[" << count - 1 - n << "]));\n";
        }
        n += 1;
      }
      parseTable << "\t\t\t" << production.expression[rule.first].code << ";\n";
      parseTable << "\t\t\treturn out;\n";
      parseTable << "\t\t}\n";
      parseTable << "\t\t},\n";
    }
   
    parseTable << "\t},\n";

    parseTable << "},\n";
  }
  parseTable << "};\n}\n";

  return fragments::format(
    fragments::Parser,
    { grammar.ast.tokens.size() > 0 ? fragments::TokenizerIncludes : "",
      grammar.ast.productions[0].symbol,
      tokenize,
      parseTable.str(),
      grammar.ast.productions[0].symbol,
      grammar.ast.productions[0].symbol
    });
}

int main(int argc, char* argv[]) {
  std::string source = argv[1];
  std::string outDir = argv[2];

  Grammar grammar = Grammar::Create(ReadFile(source));

  EmitFile(outDir + "Parser.generated.h", GenHeader(grammar));
  EmitFile(outDir + "Parser.generated.cpp", GenParser(grammar));

  return 0;
}