#include "CNFParser.h"
#include <string>

unique_ptr<CNFFormula> CNFParser::parse(WordReader& reader) {
	unique_ptr<vector<string>> line;
	unique_ptr<CNFFormula> formula = nullptr;
	size_t clause = 0;
	while ((line = move(reader.readLine())) != nullptr) {
		if (line->size() == 0 || line->at(0) == "c") continue;
		else if (line->size() == 4 && line->at(0) == "p" && line->at(1) == "cnf") {
			if (formula == nullptr) formula = make_unique<CNFFormula>(stoi(line->at(2)), stoi(line->at(3)));
		}
		else {
			if (formula != nullptr) {
				for (size_t i = 0; i < line->size() - 1; i++) {
					int var = stoi(line->at(i));
					formula->addVar(clause, var);
				}
			}
			clause++;
		}
	}
	return move(formula);
}
