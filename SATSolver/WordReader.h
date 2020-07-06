#pragma once
#include <sstream>
#include <vector>

using namespace std;

class WordReader {
public:
	WordReader(istream& input, char separator) : input_(input), separator_(separator) {};
	unique_ptr<vector<string>> readLine();
private:
	char readChar = NULL;
	char separator_;
	istream& input_;
	void readThroughSeparators();
	string readWord();
};

