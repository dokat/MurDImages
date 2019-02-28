#include "touchBoard.h"

ofEvent<void> touchBoard::backToLettersEvent = ofEvent<void>();
ofEvent<void> touchBoard::zoomOutEvent = ofEvent<void>();
ofEvent<void> touchBoard::zoomInEvent = ofEvent<void>();
ofEvent<void> touchBoard::centerEvent = ofEvent<void>();
ofEvent<string> touchBoard::keywordSelectedEvent = ofEvent<string>();
ofEvent<string> touchBoard::keywordDeletedEvent = ofEvent<string>();

touchBoard::touchBoard()
{	
	//ofLogNotice("touchBoard::touchBoard");
	//Chargement de la police
	//ofTrueTypeFont::Settings settings("Hanken-Light.ttf", 30);
	ofTrueTypeFontSettings settings("Hanken-Light.ttf", 30);
	settings.antialiased = true;
	settings.addRange(ofUnicode::Latin);
	settings.addRange(ofUnicode::Latin1Supplement);
	settings.addRange(ofUnicode::LatinExtendedAdditional);
	myFont.load(settings);
	myFont.setLineHeight(34.0f);
	myFont.setLetterSpacing(1.035);

	//back and next keys
	nextKey = new key(LABEL_NEXT, &myFont, 0, 0, false);
	nextKey->dispatchEvent = false;
	//nextKey->staticPosition = true;
	nextVisible = false;

	backKey = new key(LABEL_RETOUR, &myFont, 0, 0, false);
	backKey->dispatchEvent = false;
	//backKey->staticPosition = true;
	backVisible = false;

	//zoom keys
	zoomInKey = new key(LABEL_ZOOMIN, &myFont, 0, 0, false);
	zoomInKey->staticPosition = true;
	zoomInKey->dispatchEvent = false;
	zoomInKey->fontColor = ofColor(255);
	ofAddListener(zoomInKey->keyPressed, this, &touchBoard::onZoomInPressed);

	zoomOutKey = new key(LABEL_ZOOMOUT, &myFont, 0, 0, false);
	zoomOutKey->staticPosition = true;
	zoomOutKey->dispatchEvent = false;
	zoomOutKey->fontColor = ofColor(255);
	ofAddListener(zoomOutKey->keyPressed, this, &touchBoard::onZoomOutPressed);

	//center key
	centerKey = new key(LABEL_CENTER, &myFont, 0, 0, false);
	centerKey->staticPosition = true;
	centerKey->dispatchEvent = false;
	centerKey->fontColor = ofColor(255);
	ofAddListener(centerKey->keyPressed, this, &touchBoard::onCenterPressed);

	//Labels
	lblKeywordsPositionY = myFont.getStringBoundingBox(LABEL_SELECT, 0, 0).height + 5;
	lblSelectedKeywordsPositionY = myFont.getStringBoundingBox(LABEL_DELETE, 0, 0).height + 5;

	currentPage = 0;
	numPages = 0;

	ofAddListener(key::staticKeyPressed, this, &touchBoard::onKeyPressed);
}

touchBoard::~touchBoard()
{
}

void touchBoard::setDimension(int width, int height)
{
	ofLogNotice("touchBoard::setDimension");
	this->width = width;
	this->height = height;

	//place le touchBoard dans la partie inférieure de l'écran
	setPosition(0, ofGetHeight() - height);

	//positionne les boutons retour et suivant dans les coins inférieurs
	nextKey->setPosition(width-nextKey->getWidth(), y->getValue() + height - nextKey->getHeight());
	backKey->setPosition(0, y->getValue() + height - nextKey->getHeight());

	x->update();

	centerKey->setPosition(width - centerKey->getWidth() - 10, ofGetHeight() / 2 - centerKey->getHeight() / 2);
	zoomInKey->setPosition(width - zoomInKey->getWidth()-10, ofGetHeight() / 2 - centerKey->getHeight() * 2);
	zoomOutKey->setPosition(width - zoomOutKey->getWidth()-10, ofGetHeight() / 2 + centerKey->getHeight());
	
}

void touchBoard::clear()
{
	keys.clear();
	key::reset();
}

void touchBoard::addAllKey(vector<char> keys)
{
	isLetterPad = true;
	for (const auto &key : keys)
	{
		addKey(key);
	}
	sort();
}

void touchBoard::addAllKey(vector<string> keys)
{
	isLetterPad = false;
	for (const auto key : keys)
	{
		//ofLogNotice("touchBoard::addAllKey") << key;
		addKey(key, false);
	}
	sort();
}


/**
 * \brief Ajoute une touche au clavier.
 * \param charLabel Libellé (juste une lettre) de la touche.
 */
void touchBoard::addKey(char charLabel)
{
	addKey(string(1, charLabel), true);
}

/**
 * \brief Ajoute une touche au clavier.
 * \param stringLabel Libellé de la touche.
 * \param sameSize Si true alors toutes les touches doivent avoir la même taille.
 */
void touchBoard::addKey(string stringLabel, bool sameSize)
{
	key * newKeyword = new key(stringLabel, &myFont, ofGetWidth() / 2, ofGetHeight(), sameSize);
	//newKeyword->setup(stringLabel, &myFont, ofGetWidth()/2, ofGetHeight(), sameSize);
	keys.push_back(newKeyword);
}


/**
 * \brief Organise les touches du clavier dans l'espace.
 */
void touchBoard::sort()
{
	const float maxLabelHeight = key::maxHeight;

	numPages = 0;
	int posX = x->getValue() + PADDING_LEFT;
	int posY = y->getValue() + PADDING_TOP + lblKeywordsPositionY;

	const int limitY = y->getValue() + height - maxLabelHeight;

	for (auto * key : keys)
	{
		if (posX + key->getWidth() > (numPages + 1) * width)
		{
			if ((posY + 2 * maxLabelHeight + 50) > limitY) {
				++numPages;
				posX = x->getValue() + 25 + (numPages * width);
				posY = y->getValue() + 25 + lblKeywordsPositionY;
			}
			else {
				posY = posY + maxLabelHeight + PADDING_LEFT;
				posX = x->getValue() + PADDING_TOP + (numPages * width);
			}
		}
		key->moveTo(posX, posY);
		posX += key->getWidth() + 25;
	}
	setCurrentPage(0);
}


void touchBoard::sortFilter()
{
	const float maxLabelHeight = key::maxHeight;

	int posX = x->getValue() + PADDING_LEFT;
	int posY = lblSelectedKeywordsPositionY + PADDING_TOP;

	//const int limitY = y + height - maxLabelHeight;

	for (auto * key : selectedKeywords)
	{
		if (posX + key->getWidth() > width)
		{
			/*if ((posY + 2 * maxLabelHeight + 50) > limitY) {
				++numPages;
				posX = x + 25 + (numPages * width);
				posY = y + 25;
			}
			else {*/
				posY = posY + maxLabelHeight + 25;
				posX = x->getValue() + 25;
			//}
		}
		//ofLogNotice("touchBoard::sortFilter") << key->getLabel() << " : " << posX << ", " << posY;
		key->moveTo(posX, posY);
		posX += key->getWidth() + 25;
	}
}


void touchBoard::setCurrentPage(int page)
{
	if (currentPage != page) {
		int previousPage = currentPage;
		currentPage = page;

		const int dx = (previousPage - currentPage) * width;
		for (key * key : keys)
		{
			key->translate(dx, 0);
		}
	}

	//affiche le bouton "mots-clés suivants" s'il existe plusieurs pages de mots-clés
	nextVisible = numPages > 0 && page < numPages;

	backVisible = (isLetterPad && page > 0) || (!isLetterPad && page > -1);
}

void touchBoard::refresh()
{
	update();

	//ofLogNotice("touchBoard::refresh") << "keys";
	for (key * key : keys)
	{
		//TODO : ça fait un peu bidouille. Sinon erreur mémoire lors de l'update (la liste peut potentiellement être effacée au même moment)
		if (!hasToRefresh) {			
			break;
		}
		key->update();
	}
	for (key * key : selectedKeywords)
	{
		key->update();
	}
	nextKey->update();
	backKey->update();
	zoomInKey->update();
	zoomOutKey->update();
	centerKey->update();

	hasToRefresh = true;
}

void touchBoard::draw()
{
	if (!selectedKeywords.empty()) {
		//Affichage du cadre de mots-clés sélectionnés.
		//cadre
		ofFill();
		ofSetColor(255, 255, 255, 200);
		ofDrawRectangle(0, 0, width, 150);
		ofSetColor(255);
		ofNoFill();
		ofDrawRectangle(0, 0, width, 150);
		//label
		ofSetColor(0);
		myFont.drawString(LABEL_DELETE, PADDING_LEFT, lblSelectedKeywordsPositionY);
		//mots-clé
		ofSetColor(255);
		for (key * key : selectedKeywords)
		{
			key->draw();			
		}
	}

	if (selectedKeywords.size() < 4) {
		/*
		 Affichage du cadre de mots-clés à sélectionner.
		*/
		//cadre
		ofFill();
		ofSetColor(255, 255, 255, 200);
		ofDrawRectangle(0, y->getValue(), width, height);
		ofSetColor(255);
		ofNoFill();
		ofDrawRectangle(0, y->getValue(), width, height);
		//label
		ofSetColor(0);
		myFont.drawString(LABEL_SELECT, PADDING_LEFT, y->getValue() + lblKeywordsPositionY);
		//mots-clés
		ofSetColor(255);
		for (key * key : keys)
		{
			key->draw();
		}

		//Affichage du bouton "retour"
		if (backVisible) {
			backKey->draw();
		}
		//Affichage du bouton "suivant"
		if (nextVisible) {
			nextKey->draw();
		}
	}

	zoomInKey->draw();
	zoomOutKey->draw();
	centerKey->draw();
}

/**
 * Gestion du clic de la souris aux coordonnées x, y
 */
bool touchBoard::mousePressed(int x, int y)
{
	bool isPressed = false;

	//retour arrière
	if(backVisible && backKey->press(x, y))
	{
		ofLogNotice("touchBoard::mousePressed") << "back : isLetterPad"  << isLetterPad << ", currentPage: " << currentPage;
		isPressed = true;
		if(!isLetterPad && currentPage == 0)
		{
			ofNotifyEvent(backToLettersEvent);
		}
		else {
			setCurrentPage(currentPage - 1);
		}
	} //mots-clés suivants
	else if(nextVisible && nextKey->press(x, y))
	{
		isPressed = true;
		setCurrentPage(currentPage + 1);
	} else if(zoomInKey->press(x, y))
	{
		isPressed = true;
	} else if(zoomOutKey->press(x, y))
	{
		isPressed = true;
	} else if(centerKey->press(x, y))
	{
		isPressed = true;
	}
	else { //sinon
		string selectedWord;
		for (auto it = keys.begin(); it != keys.end() && !isPressed; ) {
			isPressed = (*it)->press(x, y);
			if (!isLetterPad && isPressed)
			{
				selectedWord = (*it)->getLabel();
				selectedKeywords.push_back(*it);
				//(*it)->setPosition(100, 100);				
				it = keys.erase(it); //pas nécessaire si on efface tout le clavier après...				
			}
			else {
				++it;
			}
		}
		
		if (isPressed)
		{
			//On ne peut envoyer l'événement qu'après avoir parcouru la liste. Sinon erreur.
			if (!isLetterPad) {
				sortFilter();
				ofLogNotice("touchBoard::mousePressed") << "keywordSelectedEvent";
				ofNotifyEvent(keywordSelectedEvent, selectedWord);
			}
		} else //si aucun mot-clé sélectionné, vérifie les mots-clé déjà sélectionnés.
		{
			for (auto it = selectedKeywords.begin(); it != selectedKeywords.end() && !isPressed; ) {
				isPressed = (*it)->press(x, y);
				if (isPressed)
				{
					selectedWord = (*it)->getLabel();
					it = selectedKeywords.erase(it);
				}
				else {
					++it;
				}
			}
			if (isPressed)
			{
				sortFilter();
				ofNotifyEvent(keywordDeletedEvent, selectedWord);
			}
		}
	}
	return isPressed;
}

void touchBoard::mouseReleased()
{
	for (key * key : keys)
	{
		key->release();
	}
	nextKey->release();
	backKey->release();
	zoomInKey->release();
	zoomOutKey->release();
	centerKey->release();
}

void touchBoard::onTimeAnimationFinished() {

}

void touchBoard::onAlphaAnimationFinished() {
	//ofNotifyEvent(keywordPressed, *this);
}

void touchBoard::onKeyPressed(key &key) {
	ofLogNotice("touchBoard::onKeyPressed") << "keywordSelectedEvent : " << key.getLabel();
	hasToRefresh = false;
	string lbl = key.getLabel();
	ofNotifyEvent(keywordSelectedEvent, lbl);
}

void touchBoard::onZoomInPressed(key &key)
{
	ofNotifyEvent(zoomInEvent);
}

void touchBoard::onZoomOutPressed(key &key)
{
	ofNotifyEvent(zoomOutEvent);
}

void touchBoard::onCenterPressed(key &key)
{
	ofNotifyEvent(centerEvent);
}

void touchBoard::show()
{
	ofLogNotice("touchBoard::show") << "visible: " << visible;
	if (!visible) {
		visible = true;
		int dy = ofGetHeight() - height - y->getValue();
		translate(0, dy);
		for (key * key : keys)
		{
			key->translate(0, dy);
		}
		nextKey->translate(0, dy);
		backKey->translate(0, dy);
	}
}

void touchBoard::hide()
{
	ofLogNotice("touchBoard::hide") << "visible: " << visible;
	if (visible) {
		visible = false;
		int dy = ofGetHeight() - y->getValue();
		for (key * key : keys)
		{
			key->translate(0, dy);
		}
		translate(0, dy);
		nextKey->translate(0, dy);
		backKey->translate(0, dy);
	}
}