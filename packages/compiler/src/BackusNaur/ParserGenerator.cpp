#include "ParserGenerator.h"

namespace BackusNaur {

std::string GenerateParser(Grammar& grammar) {
	std::stringstream parser{};

	parser << "enum class TokenType {" << std::endl;
	for (auto terminal : grammar.Terminals()) {
		parser << '\t' << terminal << ",\n";
	}
	parser << "};" << std::endl;


	return parser.str();
}

}