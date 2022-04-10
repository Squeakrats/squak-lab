#include "json.h"
#include "Parser.generated.h"
#include <assert.h>

namespace json {

Value Create(ast::Value& ast) {
	if (std::holds_alternative<std::string>(ast.value)) {
		std::string value = std::get<std::string>(ast.value);

		return Value{ value.substr(1, value.size() - 2) };
	}
	else if (std::holds_alternative<bool>(ast.value)) {
		return Value{ std::get<bool>(ast.value) };
	}
	else if (std::holds_alternative<double>(ast.value)) {
		return Value{ std::get<double>(ast.value) };
	}

	// unhandled json value
	throw std::exception();
}

Object Create(ast::Object& ast) {
	Object object{};

	std::shared_ptr<ast::ObjectEntries> current = ast.entries;
	while (current != nullptr) {
		std::string key = current->entry->key.substr(1, current->entry->key.size() - 2);
		object.entries.insert(std::make_pair(key, Create(*current->entry->value)));
		current = current->rhs;
	}

	return object;
}

Object Create(std::string source) {
	TokenStream<Token> stream(source, Tokenize);
	ParserContext context{ stream.Next(), stream };

	return Create(*Parsejson(context));
}

};