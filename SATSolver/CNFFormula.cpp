#include "CNFFormula.h"

CNFFormula::CNFFormula(size_t vars, size_t clauses) : vars_(vars), clauses_(clauses) {
	for (size_t i = 0; i < clauses; i++) {
		structure_.push_back(vector<int>());
	}
}
