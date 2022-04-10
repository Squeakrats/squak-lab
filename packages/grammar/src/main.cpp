#include <iostream>
#include <fstream>
#include "Grammar.h"
#include "fragments.h"

std::string GenHeader(Grammar& grammar) {
	std::stringstream tokens{};
	for (auto terminal : grammar.Terminals()) {
		tokens << '\t' << terminal << ",\n";
	}

	std::stringstream declarations{};
	for (auto production : grammar.ast.productions) {
		declarations << production.type << " Parse" << production.symbol << "(ParserContext& context);\n";
	}

	return fragments::format(fragments::Header, {
		grammar.ast.code,
		grammar.ast.productions[0].symbol,
		tokens.str(),
		declarations.str()
	});
}

std::string GenParser(Grammar& grammar) {
	auto terminals = grammar.Terminals();

	std::string tokenize{};
	if (grammar.ast.tokens.size() > 0) {
		std::stringstream tokens{};
		for (auto token : grammar.ast.tokens) {
			if (token.first != "") {
				tokens << "\t\t TokenType::" << token.first << ",\n";
			}
			else {
				tokens << "\t\t std::nullopt,\n";
			}
		}

		std::stringstream expressions{};
		for (auto token : grammar.ast.tokens) {
			expressions << "\t\t\"" << token.second << "\",\n";
		}

		tokenize = fragments::format(fragments::Tokenize, {
			tokens.str(),
			expressions.str()
		});
	}

	std::stringstream parsers{};
	for (auto production : grammar.ast.productions) {
		auto rules = grammar.Rules(production.symbol);

		std::stringstream body{};
		for (auto rule : rules) {
			for (auto symbol : rule.second) {
				body << "\t\tcase TokenType::" << symbol << ":\n";
			}

			size_t paramaters = 0;
			body << "\t\t{\n";
			for (auto symbol : production.expression.at(rule.first).symbols) {
				if (terminals.find(symbol) != terminals.end()) {
					body << "\t\t\tassert(context.token.first == TokenType::" << symbol << ");\n";
					body << "\t\t\tauto P" << paramaters++ << " = context.Use();\n";
				}
				else {
					body << "\t\t\tauto P" << paramaters++ << " = Parse" << symbol << "(context); \n";
				}
			}
			body << "\n\t\t\t{" << production.expression.at(rule.first).code << "}\n";
			body << "\t\t}\n";
		}

		parsers << fragments::format(fragments::ParserImplementation, {
			production.type,
			production.symbol,
			body.str()
		});
	}

	return fragments::format(fragments::Parser, {
		grammar.ast.tokens.size() > 0 ? fragments::TokenizerIncludes : "",
		grammar.ast.productions[0].symbol,
		tokenize,
		parsers.str()
	});
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

	std::string outDir(argv[2]);
	EmitFile(outDir + "Parser.generated.h", GenHeader(grammar));
	EmitFile(outDir + "Parser.generated.cpp", GenParser(grammar));

    return 0;
}