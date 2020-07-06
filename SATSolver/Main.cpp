#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "Result.h"
#include "SatSolver.h"
#include "CNFFormula.h"
#include "WordReader.h"
#include "CNFParser.h"

using namespace std;

int main(int argc, char** argv) {
	vector<string> args(argv + 1, argv + argc);
	unique_ptr<WordReader> reader = nullptr;
	string inputFile = (args.size() != 1) ? "" : args[0];
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
	solver.printResult(*solver.satisfy(*formula), cout);
	return SUCCESS;
}