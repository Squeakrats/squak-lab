#include <iostream>
#include <fstream>
#include "Grammar.h"

std::string GenerateParser(Grammar& grammar) {
	std::stringstream parser{};

	parser << "#pragma once\n";
	parser << "#include <vector>\n";
	parser << "#include <map>\n";
	parser << "#include <utility>\n";
	parser << "#include <sstream>\n";
	parser << "\n";
	parser << "namespace " << grammar.ast[0].first << " {\n";
	parser << "\n";

	parser << "enum class TokenType {\n";
	for (auto terminal : grammar.Terminals()) {
		parser << '\t' << terminal << ",\n";
	}
	parser << "};\n";
	parser << "\n";
	parser << "using Token = std::pair<TokenType, std::string>;\n";
	parser << "\n";
	parser << "Token Tokenize(std::stringstream& source);\n";
	parser << "\n";

	parser << "std::vector<std::map<std::string, size_t>> rules = {\n";

	for (auto production : grammar.ast) {
		parser << "\t{\n\t\t";
		parser << production.first << ",\n\t\t{ ";
		for (auto rule : grammar.Rules(production.first)) {
				parser << "{ TokenType::" << rule.first << ", " << rule.second << " }, ";
		}
		parser << " },\n";
		parser << "\t},\n";
	}
	
	parser << "\n";
	parser << "};\n";


	return parser.str();
}

int main(int argc, char* argv[]) {
	std::ifstream file(argv[1]);
	std::stringstream contents{};
	
	contents << file.rdbuf();

	Grammar grammar = Grammar::Create(contents.str());
	
    std::cout << GenerateParser(grammar) << std::endl;

	// std::cout << grammar.Rules() << std::endl;

    return 0;
}