#pragma once
#include <vector>
#include <string>
#include "ofLog.h"

using namespace std;

class word {
public:
	word(string value);

	string value;
	vector<word*> linkedWords;

	void addLinkedWord(word * otherWord);
	bool isLinkedTo(word * otherWord);
	bool isLinkedTo(string otherWord);

	string toString();

	bool operator==(const word& otherWord) const
	{
		return this->value == otherWord.value;
	}

	bool operator < (const word& otherWord) const { return this->value < otherWord.value; }

	friend ostream& operator<< (ostream &strm, const word& word)
	{
		return strm << "label [" << word.value << "]";
	}
};