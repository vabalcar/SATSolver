#include "SatSolver.h"

unique_ptr<vector<bool>> SatSolver::satisfy(const CNFFormula& formula) {
	vector<vector<int>> structure = formula.getStructure();
	vector<bool> result = dpll(structure, vector<bool>(formula.getVars()));
	return make_unique<vector<bool>>(result);
}

void SatSolver::printResult(vector<bool>& result, ostream& out) const {
	if (result.size() != 0) {
		out << "SAT" << endl;
		for (size_t i = 0; i < result.size(); i++) {
			out << (result[i] ? "" : "-") << i + 1 << ' ';
		}
		out << 0;
	}
	else {
		out << "UNSAT";
	}
	out << endl;
}

vector<bool> SatSolver::dpll(vector<vector<int>> formula, vector<bool> result) {
	unitPropagation(formula, result);
	if (containsEmptyClause(formula)) return vector<bool>();
	if (isEmpty(formula)) return result;
	int l = getNextUnsatisfiedLiteral(formula);
	result[l - 1] = true;
	vector<bool> finalResult = dpll(setTrueAndSimplify(formula, l), result);
	if (finalResult.size() != 0) return finalResult;
	result[l - 1] = false;
	finalResult = dpll(setTrueAndSimplify(formula, -l), result);
	if (finalResult.size() != 0) return finalResult;
	return vector<bool>();
}

void SatSolver::unitPropagation(vector<vector<int>>& formula, vector<bool>& result) {
	int l;
	while ((l = getUnitClauseLiteral(formula)) != 0) {
		formula = setTrueAndSimplify(formula, l);
		result[abs(l) - 1] = l > 0;
	}
}

int SatSolver::getUnitClauseLiteral(const vector<vector<int>>& formula) {
	int cSize;
	for (size_t c = 0; c < formula.size(); c++) {
		cSize = 0;
		for (size_t l = 0; l < formula[c].size(); l++) {
			if (formula[c][l] != 0) cSize++;
		}
		if (cSize == 1) return formula[c][0];
	}
	return 0;
}

vector<vector<int>> SatSolver::setTrueAndSimplify(vector<vector<int>> formula, int i) {
	for (size_t c = 0; c < formula.size(); c++) {
		for (size_t l = 0; l < formula[c].size(); l++) {
			if (formula[c][l] == i) {
				formula[c] = vector<int>();
				break;
			}
			else if (formula[c][l] == -i) {
				formula[c][l] = 0;
			}
		}
	}
	return formula;
}

bool SatSolver::containsEmptyClause(const vector<vector<int>>& formula) {
	int nSize;
	for (size_t c = 0; c < formula.size(); c++) {
		nSize = 0;
		for (size_t l = 0; l < formula[c].size(); l++) {
			if (formula[c][l] == 0) nSize++;
		}
		if (nSize != 0 && nSize == formula[c].size()) return true;
	}
	return false;
}

bool SatSolver::isEmpty(const vector<vector<int>>& formula) {
	for (size_t i = 0; i < formula.size(); i++) {
		if (formula[i].size() > 0) return false;
	}
	return true;
}

int SatSolver::getNextUnsatisfiedLiteral(const vector<vector<int>>& formula) {
	for (size_t c = 0; c < formula.size(); c++) {
		for (size_t l = 0; l < formula[c].size(); l++) {
			if (formula[c][l] != 0) return abs(formula[c][l]);
		}
	}
	return 0;
}
