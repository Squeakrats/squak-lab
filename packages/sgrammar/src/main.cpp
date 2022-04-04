#include <iostream>
#include <fstream>
#include "Grammar.h"

std::string GenerateParser(Grammar& grammar) {
	std::stringstream parser{};

	parser << "#pragma once\n";
	parser << "#include <utility>\n";
	parser << "#include <sstream>\n";
	parser << "\n";
	parser << "namespace " << grammar.Root() << " {\n";
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

	parser << "};\n";


	return parser.str();
}

int main(int argc, char* argv[]) {
	std::ifstream file(argv[1]);
	std::stringstream contents{};
	
	contents << file.rdbuf();

	Grammar grammar = Grammar::Create(contents.str());
	
    std::cout << GenerateParser(grammar) << std::endl;

    return 0;
}