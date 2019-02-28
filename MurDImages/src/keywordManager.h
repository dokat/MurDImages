#pragma once

#include "ofUtils.h"
#include "ofLog.h"
#include <vector>
#include <map>
#include "singleton.h"
#include "image.h"
#include "word.h"

using namespace std;

class keywordManager : public singleton<keywordManager>
{
	friend class singleton<keywordManager>;

private:
	keywordManager();
	~keywordManager();

public:
	void extractKeywordsFromImage(image * img);
	vector<char> getKeys();
	vector<string> getValues(char key);
	void addWordToFilter(string word);
	void removeWordFromFilter(string word);
	int getFilterSize();
	string toString();

	map<char, vector<word*>> fullDictionary;
	int nKeywords = 0;

private :
	bool isInFilter(word * value);
	bool isLinkedToFilter(word * value);
	void addKeywordToList(string wordToAdd, vector<word*> * newWords);
	void createLinks(vector<word*> * linkedWords);

	vector<string> filters;
};

