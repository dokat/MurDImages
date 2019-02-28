#include "pictureCloud.h"

pictureCloud::pictureCloud()
{
	pictureManager = pictureManager::getInstance();
	scale = new easingValue<float>(0.3, 3);
	ofAddListener(scale->easingFinished, this, &pictureCloud::onZoomOutFinished);

	recenter = new easingValue<float>(0.0, 3);
	ofAddListener(recenter->easingFinished, this, &pictureCloud::onRecenterFinished);

	//TODO : essayer d'utiliser un easingvalue pour la position
	pos = new easingValue<ofVec2f>(ofVec2f(0, 0), 2);
	position = ofVec2f(0, 0);
	velocity = ofVec2f(0, 0);

	center.position = ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2);
	center.strength = 1;
}

pictureCloud::~pictureCloud()
{
}

void pictureCloud::init()
{
	for (auto &img : pictureManager->mesImages) {
		allThePictures.push_back(&img);
		picturesToDisplay.push_back(&img);
		auto* s = new spring(&center, &img);
		springs.push_back(s);
	}
	ofLogNotice("pictureCloud::init") << "Affiche " << picturesToDisplay.size() << " images.";
}

void pictureCloud::reset()
{
	keywordsFilter.clear();
}

bool pictureCloud::press(int x, int y, image ** imageToDisplay)
{
	//ofLogNotice("pictureCloud::press") << x << ";" << y;
	//size_t i = 0;
	bool found = false;

	auto it = picturesToDisplay.begin();
	while (it != picturesToDisplay.end() && !found) {
		//image * img = picturesToDisplay.at(i);
		if (x > position.x + (*it)->getCenter().x && x < position.x + (*it)->getCenter().x + (*it)->size.x &&
			y > position.y + (*it)->getCenter().y && y < position.y + (*it)->getCenter().y + (*it)->size.y)
		{
			*imageToDisplay = *it;
			found = true;
			//TODO: faire passer l'image sur laquelle on clic devant
			//swap(picturesToDisplay.at(picturesToDisplay.size()-1), *it);
			//ofLogNotice("pictureCloud::press") << "Swap" << (*it)->getFilename();
			picturesToDisplay.push_back(*it);
			it = picturesToDisplay.erase(it);
		}
		else {
			++it;
		}
	}

	return found;
}

/**
 * Cette fonction ajoute un mot-clé au filtre de mots-clé et supprime toutes les images
 * ne possédant pas l'ensemble des mots-clé présents dans le filtre.
 */
void pictureCloud::addKeywordToFilter(string keyword)
{
	ofLogNotice("pictureCloud::addKeywordToFilter") << keyword;
	string keywordToFind = utils::utf8ToString(keyword);
	keywordsFilter.push_back(keywordToFind);

	for (auto it = picturesToDisplay.begin(); it != picturesToDisplay.end();) {
		if (!(*it)->contains(keywordsFilter)) {
			it = picturesToDisplay.erase(it);
		}
		else {
			++it;
		}
	}
	ofLogNotice("pictureCloud::addKeywordToFilter") << "Affiche " << picturesToDisplay.size() << " images.";

	recenterCloud();
}

void pictureCloud::recenterCloud()
{
	//ofLogNotice("pictureCloud::recenterCloud");
	//recenter->setNewValue(1.0f);

	position = ofVec2f(0, 0);
	velocity = ofVec2f(0, 0);
	
	for (auto img : picturesToDisplay)
	{
		//img->position.x = ofRandomWidth() / 2 + ofGetWidth() / 4;
		//img->position.y = ofRandomHeight() / 2 + ofGetHeight() / 4;
		img->position.x = ofGetWidth() / 4 + ofRandomWidth() / 2;
		img->position.y = ofGetHeight() / 4 + ofRandomHeight() / 2;
		//img->velocity = ofVec2f(0);
	}
}

void pictureCloud::removeKeywordToFilter(string keyword)
{
	//supprime le mot de la liste des mots-clés
	string keywordToFind = utils::utf8ToString(keyword);
	{
		bool found = false;
		auto it = find(keywordsFilter.begin(), keywordsFilter.end(), keywordToFind);
		if (it != keywordsFilter.end()) {
			keywordsFilter.erase(it);
		}
	}	

	//mets à jour l'ensemble des images à afficher
	picturesToDisplay.clear();
	for (auto it = allThePictures.begin(); it != allThePictures.end();) {
		if ((*it)->contains(keywordsFilter)) {
			picturesToDisplay.push_back(*it);
		}
		++it;
	}
	ofLogNotice("pictureCloud::removeKeywordToFilter") << "Affiche " << picturesToDisplay.size() << " images.";
}

void pictureCloud::update() {
	scale->update();
	recenter->update();

	
	/*for (auto &node : picturesToDisplay) {
		center.attract(&picturesToDisplay);
	}*/
	if (recenter->getValue() > 0) {
		
		/*for (auto &spring : springs)
		{
			spring->update();
		}*/
	} else
	{
		for (auto &img : picturesToDisplay)
		{
			img->attract(&picturesToDisplay);
		}
	}
	for (auto &img : picturesToDisplay)
	{		
		img->update();
	}

	position += velocity;
	velocity *= (1 - dampling);

	//permet de rester dans les coordonnées limites des photos
	if (position.x > maxX)
	{
		position.x = maxX;
	}
	if (position.y> maxY)
	{
		position.y = maxY;
	}
	if (position.x < minX)
	{
		position.x = minX;
	}
	if (position.y < minY)
	{
		position.y = minY;
	}
}

void pictureCloud::draw()
{
	//TODO : peut-être nécessaire pour repasser des photos l'une devant l'autre
	//glEnable(GL_DEPTH_TEST);
	pictureManager->atlasManager.beginBatchDraw();
	/*for (auto &img : picturesToDisplay)
	{
		drawPicture(img);
	}*/
	for(size_t i = picturesToDisplay.size() ; i-- > 0 ; )
	{
		drawPicture(picturesToDisplay.at(i));
	}
	pictureManager->atlasManager.endBatchDraw();
}

void pictureCloud::drawPicture(image * img)
{
	float s = pictureManager::ATLAS_PICTURE_SIZE * scale->getValue();
	TextureAtlasDrawer::TextureDimensions td = pictureManager->atlasManager.getTextureDimensions(img->getFilename());
	float width = s * td.aspectRatio;
	float height = s;
	img->setSize(width, height);
	ofRectangle r = ofRectangle(img->getCenter()+position, width, height);
	pictureManager->atlasManager.drawTextureInBatch(img->getFilename(), r);

	minY = min(r.y, minY);
	minX = min(r.x, minX);
	maxY = max(r.y+height, maxY);
	maxX = max(r.x+width, maxX);
}

void pictureCloud::windowResized(int w, int h)
{
	center.position = ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2);

	for (auto img : picturesToDisplay)
	{
		img->position.x = ofRandomWidth();
		img->position.y = ofRandomHeight();
	}
}

void pictureCloud::mouseDragged(int dx, int dy)
{
	position += ofVec2f(dx, dy);
}

void pictureCloud::setSpeed(int dx, int dy)
{
	velocity = ofVec2f(dx, dy);
}

void pictureCloud::onZoomOutFinished()
{
	ofLogNotice("pictureCloud::onZoomOutFinished");
	//ofRemoveListener(scale->easingFinished, this, &pictureCloud::onZoomOutFinished);
	recenter->setCurrentValue(0);
}

void pictureCloud::onRecenterFinished()
{
	ofLogNotice("pictureCloud::onRecenterFinished");
	recenter->setCurrentValue(0);
}

void pictureCloud::zoomIn()
{
	scale->setNewValue(scale->getValue() + ZOOM_FACTOR);
}

void pictureCloud::zoomOut()
{
	scale->setNewValue(scale->getValue() - ZOOM_FACTOR > 0.01 ? scale->getValue() - ZOOM_FACTOR : 0.01);
	//center.strength = 100;
	recenter->setCurrentValue(1);
}