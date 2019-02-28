#include "keywordManager.h"

keywordManager::keywordManager()
{
}

keywordManager::~keywordManager()
{
}

/**
* Retourne la liste des lettres qui possède au moins un mot-clé.
*/
vector<char> keywordManager::getKeys()
{
	vector<char> keys;
	for (auto it = fullDictionary.begin(); it != fullDictionary.end(); ++it)
	{
		if (!getValues(it->first).empty()) {
			keys.push_back(it->first);
		}
	}
	return keys;
}

vector<string> keywordManager::getValues(char key)
{
	//ofLogNotice("keywordManager::getValues") << key;
	vector<string> values;
	for (word * w : fullDictionary[key])
	{
		if (!isInFilter(w))
		{
			//ofLogNotice("keywordManager::getValues") << "!isInFilter";
			if (isLinkedToFilter(w)) {
				//ofLogNotice("keywordManager::getValues") << w->value;
				values.push_back(w->value);
			}
		}
	}
	sort(values.begin(), values.end());
	return values;
}

void keywordManager::extractKeywordsFromImage(image * img) {
	vector<string> representations = img->getRepresentations();
	vector<word*> newWords;
	for (const auto& word : representations)
	{
		addKeywordToList(word, &newWords);
	}
	createLinks(&newWords);
}

void keywordManager::addKeywordToList(const string wordValue, vector<word*> * newWords)
{
	if (wordValue.length() > 1) {
		string wordToInsert = ofToLower(ofTrim(wordValue));
		//On converti la chaine en utf8 sinon problème avec les accents
		string localeString = utils::utf8ToString(wordToInsert);
		
		char firstCharacter = localeString[0];
		//ofLogNotice("keywordManager::addKeywordToList") << label << " - " << localeString;
		//ofLogNotice("keywordManager::addKeywordToList") << firstCharacter << (int)firstCharacter;
		switch ((int)firstCharacter)
		{
		case -32: //'à'
			firstCharacter = 'a';
			break;
		case -23: //'é' et 'è':
			firstCharacter = 'e';
			break;
		}

		vector<word*> * wordList;
		//Si une liste liée à la 1ere lettre du mot n'existe pas encore
		if (fullDictionary.find(firstCharacter) == fullDictionary.end()) {
			wordList = new vector<word*>();
			wordList->push_back(new word(wordToInsert));
			fullDictionary[firstCharacter] = *wordList;
			++nKeywords;
		}
		else {
			wordList = &fullDictionary[firstCharacter];
			word * newWord = new word(wordToInsert);
			//vérifie que le mot n'existe pas déjà dans la liste
			bool found = false;
			auto it = wordList->begin();
			for (; it != wordList->end() && !found; )
			{
				//ofLogNotice("ofApp::addKeywordToList") << *newWord << " =? " << **it << " : " << (**it == *newWord);
				found = **it == *newWord;
				if (!found) {
					++it; //à ne pas mettre dans le for, sinon avance d'un cran en trop
				}
			}
			if(!found)
			{
				wordList->push_back(newWord);
				newWords->push_back(newWord);
				++nKeywords;
				sort(wordList->begin(), wordList->end());
			}
			else {
				//ofLogNotice("ofApp::addKeywordToList") << "it: " << **it;
				newWords->push_back(*it);
			}
		}
	}
}

void keywordManager::createLinks(vector<word*> * linkedWords) {
	int size = linkedWords->size();
	for (int i = 0; i < size; ++i) {
		word * currentWord = linkedWords->at(i);
		for (int j = 0; j < size; ++j)
		{
			if (i != j)
			{
				bool found = false;
				for (auto it = currentWord->linkedWords.begin(); it != currentWord->linkedWords.end() && !found; ++it)
				{
					//ofLogNotice("ofApp::createLinks") << *linkedWords->at(j) << " =? " << **it << " : " << (**it == *linkedWords->at(j));
					found = **it == *linkedWords->at(j);
				}
				if (!found) {
					currentWord->linkedWords.push_back(linkedWords->at(j));
				}
			}
		}
	}
}

bool keywordManager::isInFilter(word * value)
{
	bool found = false;
	for (auto it = filters.begin(); it != filters.end() && !found; ++it)
	{
		found = ((*it) == value->value);
	}
	return found;
}

bool keywordManager::isLinkedToFilter(word * value)
{
	bool isLinked = true;
	for (auto it = filters.begin(); it != filters.end() && isLinked; ++it)
	{
		isLinked = isLinked & (value->isLinkedTo(*it));
	}
	return isLinked;
}

void keywordManager::addWordToFilter(string word)
{
	filters.push_back(word);
	//ofLogNotice("keywordManager::addWordToFilter") << filters.size();
}

void keywordManager::removeWordFromFilter(string word)
{
	auto it = find(filters.begin(), filters.end(), word);
	//TODO : à tester
	if(it != filters.end())
	{
		filters.erase(it);
	}
}

int keywordManager::getFilterSize() {
	return filters.size();
}


string keywordManager::toString()
{
	string result;

	for (auto it = fullDictionary.begin(); it != fullDictionary.end(); ++it) {
		char c = it->first;
		vector<word*> words = it->second;
		for (const auto &word : words) {
			result += word->toString() + "\n";
		}
	}

	return result;
}