#include <iostream>
#include <fstream>
#include "Grammar.h"

std::string EmitHeader(Grammar& grammar) {
	std::stringstream header{};

	header << "// This file was auto-generated \n";
	header << "#pragma once\n";
	header << "#include \"TokenStream.h\"\n";
	header << grammar.ast.code;
	header << "\n";
	header << "namespace " << grammar.ast.productions[0].symbol << " {\n";
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

	for (auto production : grammar.ast.productions) {
		header << production.type << " Parse" << production.symbol << "(ParserContext& context);\n";
	}

	header << "\n}\n";

	return header.str();
}

std::string EmitParser(Grammar& grammar) {
	std::stringstream parser{};

	auto terminals = grammar.Terminals();

	parser << "// This file was auto-generated \n";
	parser << "#include <assert.h>\n";
	parser << "#include <utility>\n";
	parser << "#include \"Parser.h\"\n";
	if (grammar.ast.tokens.size() > 0) {
		parser << "#include \"RegularExpression.h\"\n";
		parser << "#include <optional>\n";
	}

	parser << "\n";
	parser << "namespace " << grammar.ast.productions[0].symbol << " {\n";
	parser << "\n";

	if (grammar.ast.tokens.size() > 0) {
		parser << "Token Tokenize(std::stringstream& stream) {\n";
		parser << "\tstatic std::vector<std::optional<TokenType>> tokens = {\n";
		for (auto token : grammar.ast.tokens) {
			if (token.first != "") {
				parser << "\t\t TokenType::" << token.first << ",\n";
			} else {
				parser << "\t\t std::nullopt,\n";
			}
		}
		parser << "\t};\n";

		parser << "\n\tstatic DFA dfa = DFA::FromNFA(RegularExpression::Create(std::vector<std::string>({\n";
		for (auto token : grammar.ast.tokens) {
			parser << "\t\t\"" << token.second << "\",\n";
		}
		parser << "\t})));\n\n";
	
		parser << "\twhile(true) {\n";
		parser << "\t\tauto longest = dfa.Longest(stream);\n";
		parser << "\t\tif (longest.second != 0) {\n";
		parser << "\t\t\tstd::optional<TokenType> token = tokens[longest.second - 1];\n";
		parser << "\t\t\tif (token != std::nullopt) {\n";
		parser << "\t\t\t\treturn std::make_pair(token.value(), longest.first); \n";
		parser << "\t\t\t}\n";
		parser << "\t\t} else {\n";
		parser << "\t\t\treturn std::make_pair(TokenType::EndOfFile, \"\");\n";
		parser << "\t\t}\n";
		parser << "\t}\n";
		parser << "}\n";
	}

	for (auto production : grammar.ast.productions) {
		auto rules = grammar.Rules(production.symbol);

		parser << production.type << " Parse" << production.symbol << "(ParserContext& context) { \n";
		parser << "\tswitch(context.token.first) {\n";
		for (auto rule : rules) {
			for (auto symbol : rule.second) {
				parser << "\t\tcase TokenType::" << symbol << ":\n";
			}

			parser << "\t\t{\n";
			
			size_t paramaters = 0;
			for (auto symbol : production.expression.at(rule.first).symbols) {
				if (terminals.find(symbol) != terminals.end()) {
					parser << "\t\t\tassert(context.token.first == TokenType::" << symbol << ");\n";
					parser << "\t\t\tauto P" << paramaters++ << " = context.Use();\n";
				}
				else {
					parser << "\t\t\tauto P" << paramaters++ << " = Parse" << symbol << "(context); \n";
				}
			}
			parser << "\n\t\t\t{" << production.expression.at(rule.first).code << "}\n";
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
	std::string source(argv[1]);

	std::ifstream file(source);
	std::stringstream contents{};
	contents << file.rdbuf();

	Grammar grammar = Grammar::Create(contents.str());

	std::string outDir(source.substr(0, source.find_last_of("\\/") + 1));
	EmitFile(outDir + "Parser.h", EmitHeader(grammar));
	EmitFile(outDir + "Parser.cpp", EmitParser(grammar));

    return 0;
}