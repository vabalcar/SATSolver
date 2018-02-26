#include "stdafx.h"
#include <iostream>
#include <vector>
#include <memory>
#include <sstream>
#include <fstream>
#include <queue>
#include <stack>

using namespace std;

class WordReader {
public:
	WordReader(istream& input, char separator) : input_(input), separator_(separator) {};
	unique_ptr<vector<string>> readLine() {
		vector<string> words;
		while (readChar != '\n' && !input_.fail()) {
			readThroughSeparators();
			words.push_back(readWord());
		}
		if (readChar == '\n') readChar = NULL;
		if (words.size() == 0) return nullptr;
		return make_unique<vector<string>>(words);
	}
private:
	char readChar = NULL;
	char separator_;
	int i_;
	istream& input_;
	void readThroughSeparators() {
		while (!input_.fail() && (readChar = input_.get()) == separator_) {}
	}
	string readWord() {
		stringstream word;
		if (readChar != NULL) word << readChar;
		while ((readChar = input_.get()) != separator_ && !input_.fail() && readChar != '\n') {
			if (readChar != '\r') word << readChar;
		}
		return word.str();
	}
};

class CNFFormula {
private:
	vector<vector<int>> structure_;
	size_t vars_, clauses_;
public:
	CNFFormula(size_t vars, size_t clauses) : vars_(vars), clauses_(clauses) {
		for (size_t i = 0; i < clauses; i++) {
			structure_.push_back(vector<int>());
		}
	}
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
	const vector<vector<int>> & getStructure() const {
		return structure_;
	}
};

/*
class ExpressionTree {
public:
	string op;
	unique_ptr<ExpressionTree> leftOperand;
	unique_ptr<ExpressionTree> rightOperand;
};

class TseitinsTransformation {
public:
	void transform(WordReader& reader, ostream& out) {
		vector<string> line = *reader.readLine();
		queue<string> formula;
		lastVar = INT_MIN;
		int tmpVar;
		for (size_t i = 0; i < line.size(); i++) {
			formula.push(line[i]);
			if (isNumber(line[i]) && (tmpVar = stoi(line[i])) > lastVar) {
				lastVar = tmpVar;
			}
		}
		vars_ = queue<int>();
		unique_ptr<ExpressionTree> exprTree = move(toExpressionTree(formula));
		vector<vector<int>> cnf = toCNF(move(exprTree));
		printInDIMACS(cnf, out);
	}
private:
	const string NEGATION = "-";
	const string CONJUCTION = "^";
	const string DISJUNCTION = "v";
	const string IMPLICATION = "->";
	const string EQUIVALANCE = "<->";
	queue<int> vars_;
	int lastVar;
	bool isNumber(const string& s) {
		for (int i = 0; i < s.size(); i++) {
			if (!isdigit(s[i])) return false;
		}
		return true;
	}
	unique_ptr<ExpressionTree> toExpressionTree(queue<string>& formula) {
		stack<unique_ptr<ExpressionTree>> operands;
		while (!formula.empty()) {
			unique_ptr<ExpressionTree> node;
			node->op = formula.front();
			formula.pop();
			if (isOperator(node->op)) {
				if (node->op == NEGATION) {
					node->leftOperand = move(operands.top());
					operands.pop();
				}
				else {
					node->leftOperand = move(operands.top());
					operands.pop();
					node->rightOperand = move(operands.top());
					operands.pop();
				}
			}
			operands.push(move(node));
		}
		return move(operands.top());
	}
	vector<vector<int>> toCNF(unique_ptr<ExpressionTree> expressionTree) {
		return vector<vector<int>>();
		deque<ExpressionTree> processQueue;
		processQueue.push_back(*move(expressionTree));
		vector<vector<int>> cnf, subCnf;
		int x1, x2, x3;
		string leftOperand, rightOperand;
		while (!processQueue.empty()) {
			const ExpressionTree & op = processQueue.front();
			processQueue.pop_front();
			rightOperand = op.rightOperand->op;
			leftOperand = op.leftOperand->op;
			x1 = getNewVar(false);
			if (cnf.empty()) {
				vector<int> firstClause;
				firstClause.push_back(x1);
				cnf.push_back(firstClause);
			}

			if (isOperator(leftOperand)) {
				processQueue.push_back(*op.leftOperand);
				x2 = getNewVar(true);
			}
			else {
				x2 = stoi(leftOperand);
			}
			
			if (op.op != NEGATION) {
				if (isOperator(rightOperand)) {
					processQueue.push_back(*op.rightOperand);
					x3 = getNewVar(true);
				}
				else {
					x3 = stoi(rightOperand);
				}
			}
			else {
				x3 = 0;//nothing
			}
			subCnf = getSubCnf(op.op, x1, x2, x3);
			for (size_t i = 0; i < subCnf.size(); ++i) {
				cnf.push_back(subCnf[i]);
			}			
		}
		return cnf;
	}
	int getNewVar(bool reusable) {
		int var;
		if (!reusable) {
			if (vars_.empty()) {
				return ++lastVar;
			}
			else {
				var = vars_.front();
				vars_.pop();
				return var;
			}
		}
		else {
			var = ++lastVar;
			vars_.push(var);
			return var;
		}
	}
	vector<vector<int>> getSubCnf(const string& subFormulaOperator, int x1, int x2, int x3) const {
		vector<vector<int>> cnf;
		if (subFormulaOperator == NEGATION) {
			cnf = { {-x1, -x2}, {x1, x2} };
		} else if (subFormulaOperator == CONJUCTION) {
			cnf = { {-x1, x2}, {-x1, x3}, {-x2, -x3, x1} };
		} else if(subFormulaOperator == DISJUNCTION) {
			cnf = { { -x1, x2, x3 }, { x1, -x2 }, { x1, -x3 } };
		} else if (subFormulaOperator == IMPLICATION) {
			cnf = { { -x1, -x2, x3 }, { x1, x2 }, { x1, -x3 } };
		} else if (subFormulaOperator == EQUIVALANCE) {
			cnf = { { -x1, -x2, x3 }, { -x1, x2, -x3 }, { x1, -x2, -x3 }, { x1, x2, x3 } };
		}
		return cnf;
	}
	bool isOperator(const string& s) const {
		return s == NEGATION || s == CONJUCTION || s == DISJUNCTION || s == IMPLICATION || s == EQUIVALANCE;
	}
	void printInDIMACS(const vector<vector<int>> cnf, ostream& out) {
		out << "p cnf " << lastVar - 1 << ' ' << cnf.size() << endl;
		for (size_t c = 0; c < cnf.size(); c++) {
			for (size_t v = 0; v < cnf[c].size(); v++) {
				out << cnf[c][v] << " ";
			}
			out << '0' << endl;
		}
	}
};
*/

class CNFParser {
public:
	static unique_ptr<CNFFormula> parse(WordReader& reader) {
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
};

enum Result {
	FAILURE = -1,
	SUCCESS = 0
};

class SatSolver {
public:
	unique_ptr<vector<bool>> satisfie(const CNFFormula& formula) {
		vector<vector<int>> structure = formula.getStructure();
		vector<bool> result = dpll(structure, vector<bool>(formula.getVars()));
		return make_unique<vector<bool>>(result);
	}
	void printResult(vector<bool> & result, ostream & out) const {
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
private:
	vector<bool> dpll(vector<vector<int>> formula, vector<bool> result) {
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
	void unitPropagation(vector<vector<int>> & formula, vector<bool> & result) {
		int l;
		while ((l = getUnitClauseLiteral(formula)) != 0) {
			formula = setTrueAndSimplify(formula, l);
			result[abs(l) - 1] = l > 0;
		}
	}
	int getUnitClauseLiteral(const vector<vector<int>> & formula) {
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
	vector<vector<int>> setTrueAndSimplify(vector<vector<int>> formula, int i) {
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
	bool containsEmptyClause(const vector<vector<int>> & formula) {
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
	bool isEmpty(const vector<vector<int>> & formula) {
		for (int i = 0; i < formula.size(); i++) {
			if (formula[i].size() > 0) return false;
		}
		return true;
	}
	int getNextUnsatisfiedLiteral(const vector<vector<int>> & formula) {
		for (size_t c = 0; c < formula.size(); c++) {
			for (size_t l = 0; l < formula[c].size(); l++) {
				if (formula[c][l] != 0) return abs(formula[c][l]);
			}
		}
		return 0;
	}
};

int main(int argc, char** argv) {
	vector<string> args(argv + 1, argv + argc);
	unique_ptr<WordReader> reader = nullptr;
	string inputFile = (args.size() != 1)? "" : args[0];
	ifstream file;
	if (inputFile == "") {
		reader = make_unique<WordReader>(cin, ' ');
		cout << endl;
	}
	else {
		file.open(inputFile);
		if (file.good()) {
			reader = make_unique<WordReader>(file, ' ');
		}
		else {
			cerr << "Input file error" << endl;
			return FAILURE;
		}
	}
	unique_ptr<CNFFormula> formula = move(CNFParser::parse(*reader));
	SatSolver solver;
	solver.printResult(*solver.satisfie(*formula), cout);
	return SUCCESS;
}
