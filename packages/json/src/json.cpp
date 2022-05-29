#include "json.h"
#include "Parser.generated.h"
#include "utility.h"

namespace json {

bool Object::has(std::string key) {
  return this->entries.find(key) != this->entries.end();
}

json::Value Create(ast::Node& node) {
  switch (node.type) {
    case ast::NodeType::Object: {
      ast::ObjectNode& objectNode = static_cast<ast::ObjectNode&>(node);

      Object result{};
      for (auto& entry : objectNode.value) {
        std::string key = entry->key.substr(1, entry->key.size() - 2);
        result.entries.insert(std::make_pair(key, Create(*entry->value)));
      }

      return json::Value{ result };
    }
    case ast::NodeType::Array: {
      ast::ArrayNode& arrayNode = static_cast<ast::ArrayNode&>(node);

      Array result{};
      for (size_t i = arrayNode.value.size(); i --> 0;) {
        result.push_back(Create(*arrayNode.value[i]));
      }

      return json::Value{ result };
    }
    case ast::NodeType::Bool:
      return json::Value{ static_cast<ast::BoolNode&>(node).value };
    case ast::NodeType::String: {
      std::string& value = static_cast<ast::StringNode&>(node).value;

      return json::Value{ value.substr(1, value.size() - 2) };
    }
    case ast::NodeType::Double:
      return json::Value{ static_cast<ast::DoubleNode&>(node).value };
    default:
      Panic("Unhandled node type");
  }
}

Object Parse(std::string source) {
  ParserContext context{ source, GetTokenizers() };

  std::unique_ptr<ast::ObjectNode> ast =
    std::unique_ptr<ast::ObjectNode>(Parsejson(context));

  return Create(*ast).get<json::Object>();
}

};