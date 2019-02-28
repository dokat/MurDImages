#include "word.h"

word::word(string value)
{
	this->value = value;
}

void word::addLinkedWord(word * otherWord)
{
	if (!isLinkedTo(otherWord)) {
		linkedWords.push_back(otherWord);
	}
}

bool word::isLinkedTo(word * otherWord)
{
	auto it = linkedWords.begin();
	bool found = false;
	while (it != linkedWords.end() && !found) {
		//ofLogNotice("label::isLinkedTo(label)") << (*it)->value << " ?= " << otherWord;
		found = ((*it)->value == otherWord->value);
		++it;
	}
	return found;
	//return isLinkedTo(otherWord->value);
}

bool word::isLinkedTo(string otherWord)
{
	auto it = linkedWords.begin();
	bool found = false;
	while (it != linkedWords.end() && !found) {
		//ofLogNotice("label::isLinkedTo(string)") << (*it)->value << " ?= " << otherWord;
		found = ((*it)->value == otherWord);
		++it;
	}
	return found;
}

string word::toString() {
	return "label [" + value + "]";
}
