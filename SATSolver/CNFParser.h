#pragma once
#include <memory>
#include "WordReader.h"
#include "CNFFormula.h"

using namespace std;

class CNFParser {
public:
	static unique_ptr<CNFFormula> parse(WordReader& reader);
};

