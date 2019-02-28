#include "projectorApp.h"

void projectorApp::setup()
{
	ofAddListener(timer->easingFinished, this, &projectorApp::onTimer);

	//récupère les infos du fichier de conf.	
	utils::getProjectorPositionAndSize(&position, &dimension);

	//ofTrueTypeFont::Settings settings("Hanken-Light.ttf", 30);
	//settings.antialiased = true;
	ofTrueTypeFontSettings settings("Hanken-Light.ttf", 30);
	settings.antialiased = true;
	settings.addRange(ofUnicode::Latin);
	settings.addRange(ofUnicode::Latin1Supplement);
	settings.addRange(ofUnicode::LatinExtendedAdditional);
	myFont.load(settings);
	myFont.setLineHeight(34.0f);
	myFont.setLetterSpacing(1.035);

	infoKey = new key("informations", &myFont, 0, 0, false);
	infoKey->dispatchEvent = false;
	infoKey->fontColor.set(0, 0, 0, 255);
	infoKey->borderColor.set(255);
	infoKey->backgroundColor.set(225);
	infoKey->setPosition(100, 1000);

	ofLogNotice("projectorApp::setup") << dimension;
	//ofSetFullscreen(true);
	ofSetWindowPosition(position.x, position.y);
	ofSetWindowShape(dimension.x, dimension.y);

	pictureManager = pictureManager::getInstance();
}

void projectorApp::update()
{
	if (currentImage != nullptr) {
		infoKey->update();

		if(infoDisplayed)
		{
			timer->update();
		}
	}
}

void projectorApp::draw()
{
	ofBackgroundGradient(ofColor(200, 200, 200), ofColor(50, 50, 50));

	if (currentImage != nullptr) {
		myImage.draw(x, y, width, height);
		ofLogNotice("projectorApp::draw") << width << ";" << height;

		if (infoDisplayed) {
			infoKey->draw();
		}
	}
}

void projectorApp::setPictureToDisplay(image * imageToDisplay)
{
	currentImage = imageToDisplay;
	myImage.load(imageToDisplay->getFilename());
	
	ofLogNotice("setPictureToDisplay") << myImage.getWidth() << ";" << myImage.getHeight();
	float ratio = myImage.getWidth() / myImage.getHeight();

	if(ratio >= 1) //largeur plus grande que hauteur
	{
		width = dimension.x - 200;
		height = width / ratio;
	} else //largeur plus petite que hauteur
	{
		height = dimension.y - 150;
		width = height * ratio;
	}
	
	x = dimension.x / 2.f - width / 2;
	y = dimension.y / 2.f - height / 2;
	ofLogNotice("setPictureToDisplay (x;y): ") << x << ";" << y;
	ofLogNotice("setPictureToDisplay (w;h): ") << width << ";" << height;

	infoKey->setLabel(imageToDisplay->numInventaire + " - " + imageToDisplay->getInscriptions());
	//infoKey->moveTo(ofGetWidth() / 2 - infoKey->getWidth(), 1000); //500

	//TODO : ajouter un dégradé alpha sur l'ensemble de l'élément
	timer->setNewValue(1);
	infoDisplayed = true;
}

void projectorApp::onTimer()
{
	infoDisplayed = false;
	timer->setCurrentValue(0);
}