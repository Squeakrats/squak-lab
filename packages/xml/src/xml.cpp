#include "xml.h"
#include "Parser.generated.h"

namespace xml {

XMLElement Create(ast::Element& element) {
  return XMLElement{};
}

 XMLElement Parse(std::string source) {
  TokenStream<Token> stream(source, Tokenize);
  ParserContext context{ stream.Next(), stream };
  ast::Element ast = Parsexml(context);

  return Create(ast);

}

};
