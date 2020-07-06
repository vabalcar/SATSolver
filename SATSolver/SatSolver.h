#pragma once
#include <iostream>
#include <vector>
#include "CNFFormula.h"

using namespace std;

class SatSolver {
public:
	unique_ptr<vector<bool>> satisfy(const CNFFormula& formula);
	void printResult(vector<bool>& result, ostream& out) const;
private:
	vector<bool> dpll(vector<vector<int>> formula, vector<bool> result);
	void unitPropagation(vector<vector<int>>& formula, vector<bool>& result);
	int getUnitClauseLiteral(const vector<vector<int>>& formula);
	vector<vector<int>> setTrueAndSimplify(vector<vector<int>> formula, int i);
	bool containsEmptyClause(const vector<vector<int>>& formula);
	bool isEmpty(const vector<vector<int>>& formula);
	int getNextUnsatisfiedLiteral(const vector<vector<int>>& formula);
};

