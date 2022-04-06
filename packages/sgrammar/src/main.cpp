#include <iostream>
#include <fstream>
#include "Grammar.h"

std::string EmitHeader(Grammar& grammar) {
	std::stringstream header{};

	header << "#pragma once\n";
	header << "#include \"TokenStream.h\"\n";
	header << "\n";
	header << "namespace " << grammar.ast[grammar.ast.size() - 1].first << " {\n";
	header << "\n";
	
	header << "enum class TokenType {\n";
	for (auto terminal : grammar.Terminals()) {
		header << '\t' << terminal << ",\n";
	}
	header << "};\n";
	header << "\n";
	header << "using Token = std::pair<TokenType, std::string>;\n";
	header << "\n";
	header << "Token Tokenize(std::stringstream& source);\n";
	header << "\n";

	header << "class ParserContext {\n";
	header << "public:\n";
	header << "\tToken token;\n";
	header << "\tTokenStream<Token>& stream;\n";
	header << "\tToken Use() { auto old = this->token; this->token = this->stream.Next(); return old; }\n";
	header << "};\n\n";

	for (auto production : grammar.ast) {
		header << production.second.second.first << " Parse" << production.first << "(ParserContext& context);\n";
	}

	header << "\n}\n";

	return header.str();
}

std::string EmitParser(Grammar& grammar) {
	std::stringstream parser{};

	auto terminals = grammar.Terminals();

	parser << "#include <assert.h>\n";
	parser << "#include <utility>\n";
	parser << "#include \"Parser.h\"\n";
	parser << "\n";
	parser << "namespace " << grammar.ast[grammar.ast.size() - 1].first << " {\n";
	parser << "\n";

	for (auto production : grammar.ast) {
		auto rules = grammar.Rules(production.first);

		parser << production.second.second.first << " Parse" << production.first << "(ParserContext& context) { \n";
		parser << "\tswitch(context.token.first) {\n";
		for (auto rule : rules) {
			for (auto symbol : rule.second) {
				parser << "\t\tcase TokenType::" << symbol << ":\n";
			}

			parser << "\t\t{\n";
			
			size_t paramaters = 0;
			for (auto symbol : production.second.first.at(rule.first)) {
				if (terminals.find(symbol) != terminals.end()) {
					parser << "\t\t\tassert(context.token.first == TokenType::" << symbol << ");\n";
					parser << "\t\t\tauto P" << paramaters++ << " = context.Use();\n";
				}
				else {
					parser << "\t\t\tauto P" << paramaters++ << " = Parse" << symbol << "(context); \n";
				}
			}
			parser << "\n\t\t\t{" << production.second.second.second << "}\n";
			parser << "\t\t}\n";
		}
 		parser << "\t\tdefault:\n";
		parser << "\t\t\treturn nullptr;\n";
		parser << "\t}\n";
		parser << "}\n";
		parser << "\n";
	}

	parser << "}\n";

	return parser.str();
}

void EmitFile(std::string name, std::string contents) {
	std::ofstream file(name);
	file << contents;
	file.close();
}

int main(int argc, char* argv[]) {
	std::ifstream file(argv[1]);
	std::stringstream contents{};
	
	contents << file.rdbuf();

	Grammar grammar = Grammar::Create(contents.str());

	std::string outputDir(argv[2]);
	EmitFile(outputDir + "Parser.h", EmitHeader(grammar));
	EmitFile(outputDir + "Parser.cpp", EmitParser(grammar));
    return 0;
}