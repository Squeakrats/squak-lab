#include "xml.h"
#include "Parser.generated.h"
#include "utility.h"

std::string trimAttribute(std::string value) {
  size_t length = 0;
  for (size_t i = value.size() - 1; i >= 0; i--) {
    if (value[i] == '"') {
      length = i - 1;
      break;
    }
  }

  return value.substr(1, length);
}

namespace xml {
std::shared_ptr<Element> CreateElement(std::shared_ptr<ast::Element>& ast);
std::shared_ptr<TextNode> CreateTextNode(std::shared_ptr<ast::TextNode>& ast);

std::shared_ptr<Node> Create(std::shared_ptr<ast::Node>& ast) {
  std::shared_ptr<ast::Element> astElement =
    std::dynamic_pointer_cast<ast::Element>(ast);
  if (astElement != nullptr) {
    return CreateElement(astElement);
  }

  std::shared_ptr<ast::TextNode> textNode =
    std::dynamic_pointer_cast<ast::TextNode>(ast);
  if (textNode != nullptr) {
    return CreateTextNode(textNode);
  }

  Panic("unhandled node type");
}

std::shared_ptr<Element> CreateElement(std::shared_ptr<ast::Element>& ast) {
  Element element{};

  for (auto& attribute : ast->openTag.attributes) {
    element.attributes.insert(std::make_pair(
      attribute.name, trimAttribute(attribute.value)));
  }

  for (auto& child : ast->children) {
    element.children.push_back(Create(child));
  }

  return std::make_shared<Element>(std::move(element));
}

std::shared_ptr<TextNode> CreateTextNode(std::shared_ptr<ast::TextNode>& ast) {
  return std::make_shared<TextNode>(ast->text);
}

Document Create(ast::Document& ast) {
  Document document{ CreateElement(ast.root) };

  return document;
}

Document Parse(std::string source) {
  TokenStream<Token> stream(source, Tokenize);
  ParserContext context{ stream.Next(), stream };
  ast::Document ast = Parsexml(context);

  return Create(ast);
}

};
