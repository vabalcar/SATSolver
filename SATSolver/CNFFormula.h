#pragma once
#include <memory>
#include <vector>

using namespace std;

class CNFFormula {
private:
	vector<vector<int>> structure_;
	size_t vars_, clauses_;
public:
	CNFFormula(size_t vars, size_t clauses);
	void addVar(int clause, int var) {
		structure_[clause].push_back(var);
	}
	unique_ptr<vector<int>> operator[](size_t i) const {
		return at(i);
	}
	unique_ptr<vector<int>> at(size_t i) const {
		return make_unique<vector<int>>(structure_[i]);
	}
	size_t getClauses() const { return clauses_; }
	size_t getVars() const { return vars_; }
	const vector<vector<int>>& getStructure() const {
		return structure_;
	}
};

