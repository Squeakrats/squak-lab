#include "json.h"
#include "Parser.generated.h"
#include "utility.h"

namespace json {

bool Object::has(std::string key) {
  return this->entries.find(key) != this->entries.end();
}

Object Create(ast::Object& ast);
Array Create(ast::Array& ast);

Value Create(ast::Value& ast) {
  if (std::holds_alternative<std::string>(ast)) {
    std::string val = std::get<std::string>(ast);

    return Value{ val.substr(1, val.size() - 2) };
  } else if (std::holds_alternative<bool>(ast)) {
    return Value{ std::get<bool>(ast) };
  } else if (std::holds_alternative<double>(ast)) {
    return Value{ std::get<double>(ast) };
  } else if (std::holds_alternative<std::shared_ptr<ast::Object>>(ast)) {
    return Value{ Create(*std::get<std::shared_ptr<ast::Object>>(ast)) };
  } else if (std::holds_alternative<std::shared_ptr<ast::Array>>(ast)) {
    return Value{ Create(*std::get<std::shared_ptr<ast::Array>>(ast)) };
  }

  Panic("unhandled json value");
}

Object Create(ast::Object& ast) {
  Object object{};

  for (ast::ObjectEntry& entry : ast.entries) {
    std::string key = entry.key.substr(1, entry.key.size() - 2);
    object.entries.insert(std::make_pair(key, Create(entry.value)));
  }

  return object;
}

Array Create(ast::Array& ast) {
  Array array{};

  for (ast::Value& element : ast.elements) {
    array.push_back(Create(element));
  }

  return array;
}

Object Parse(std::string source) {
  TokenStream<Token> stream(source, Tokenize);
  ParserContext context{ stream.Next(), stream };

  return Create(*Parsejson(context));
}

};