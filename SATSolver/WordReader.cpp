#include "WordReader.h"

unique_ptr<vector<string>> WordReader::readLine() {
	vector<string> words;
	while (readChar != '\n' && !input_.fail()) {
		readThroughSeparators();
		words.push_back(readWord());
	}
	if (readChar == '\n') readChar = NULL;
	if (words.size() == 0) return nullptr;
	return make_unique<vector<string>>(words);
}

void WordReader::readThroughSeparators() {
	while (!input_.fail() && (readChar = input_.get()) == separator_) {}
}

string WordReader::readWord() {
	stringstream word;
	if (readChar != NULL) word << readChar;
	while ((readChar = input_.get()) != separator_ && !input_.fail() && readChar != '\n') {
		if (readChar != '\r') word << readChar;
	}
	return word.str();
}
