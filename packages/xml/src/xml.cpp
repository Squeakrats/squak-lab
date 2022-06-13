#include "xml.h"
#include "Parser.generated.h"
#include "utility.h"
#include <squak/core/StringStream.h>

namespace xml {

std::shared_ptr<Element> Create(ast::ElementNode& node) {
  std::shared_ptr<Element> element = std::make_shared<Element>();
  element->tag = node.name;

  if (node.attributes != nullptr) {
    for (auto& attribute : node.attributes->value) {
      element->attributes.insert(std::make_pair(
        attribute.name, attribute.value.substr(1, attribute.value.size() - 2)));
    }
  }

  if (node.children != nullptr) {
    for (size_t i = node.children->value.size(); i-- > 0;) {
      auto& child = node.children->value[i];
      switch (child->type) {
        case ast::NodeType::Element:
          element->children.push_back(
            Create(static_cast<ast::ElementNode&>(*child)));
          break;
        case ast::NodeType::Text:
          element->children.push_back(std::make_unique<TextNode>(
            static_cast<ast::TextNode&>(*child).value));
          break;
        default:
          Panic("Unhandled Child!");
      }
    }
  }

  return element;
}

Document Create(ast::DocumentNode& node) {
  Document document{ Create(*node.root) };

  return document;
}

Document Parse(std::string source) {
  StringStream stream{ std::move(source) };
  ParserContext context{ stream, GetTokenizers() };
  std::unique_ptr<ast::DocumentNode> ast = std::unique_ptr<ast::DocumentNode>(
    static_cast<ast::DocumentNode*>(Parsexml(context)));

  return Create(*ast);
}

};
