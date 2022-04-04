#include <iostream>
#include <fstream>
#include "Grammar.h"

std::string GenerateParser(Grammar& grammar) {
	std::stringstream parser{};

	parser << "enum class TokenType {" << std::endl;
	for (auto terminal : grammar.Terminals()) {
		parser << '\t' << terminal << ",\n";
	}
	parser << "};" << std::endl;


	return parser.str();
}

int main(int argc, char* argv[]) {
	std::ifstream file(argv[1]);
	std::stringstream contents{};
	
	contents << file.rdbuf();
	
    std::cout << contents.str() << std::endl;

    return 0;
}