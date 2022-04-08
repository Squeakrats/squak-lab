#include "json.h"

namespace json {

JSON JSON::Create(std::string source) {
	TokenStream<Token> stream(source, Tokenize);
	ParserContext context{ stream.Next(), stream };

	std::shared_ptr<ast::Object> object = Parsejson(context);

	return JSON{ *object };
}

};