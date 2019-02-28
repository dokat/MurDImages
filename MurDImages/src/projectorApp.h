#pragma once
#include "ofMain.h"
#include "image.h"
#include "key.h"
#include "easingValue.h"
#include "pictureManager.h"

class projectorApp:public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();
	
	void setPictureToDisplay(image * imageToDisplay);

	ofVec2f position, dimension;

	float scale = 1;
	ofTrueTypeFont myFont;

	image * currentImage = nullptr;
	ofImage myImage;
	float x, y, width, height;

	key * infoKey;
	

	pictureManager * pictureManager;

private:
	void onTimer();

	bool infoDisplayed = false;
	easingValue<float> * timer = new easingValue<float>(0, 15);
};
