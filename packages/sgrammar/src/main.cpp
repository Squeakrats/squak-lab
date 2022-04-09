#include <iostream>
#include <fstream>
#include "Grammar.h"
#include "fragments.h"

std::string format(std::string source, std::vector<std::string> replacements) {
	std::stringstream formatted{};
	size_t size = 0;
	size_t replace = 0;

	for (size_t i = 0; i < source.size() - 1; i++) {
		if (source[i] == '{' && source[i + 1] == '}') {
			formatted << source.substr(size, i - size);
			formatted << replacements[replace++];
			i = size = i + 2;
		}
	}

	formatted << source.substr(size, source.size() - size);

	return formatted.str();
}

std::string EmitHeader(Grammar& grammar) {
	std::stringstream tokens{};
	for (auto terminal : grammar.Terminals()) {
		tokens << '\t' << terminal << ",\n";
	}

	std::stringstream declarations{};
	for (auto production : grammar.ast.productions) {
		declarations << production.type << " Parse" << production.symbol << "(ParserContext& context);\n";
	}

	return format(fragments::Header, {
		grammar.ast.code,
		grammar.ast.productions[0].symbol,
		tokens.str(),
		declarations.str()
	});
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
	
		parser << fragments::NextToken;
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