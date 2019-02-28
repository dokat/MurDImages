#pragma once
#include <string>
#include <climits>
#include <algorithm>
#include "ofGraphics.h"
#include "ofTrueTypeFont.h"
#include "ofxEasing.h"
#include "animatable.h"

class key : public animatable
{
public:
	key(std::string word, ofTrueTypeFont * font, int x, int y, bool sameSize);
	~key();

	static void reset();
	void draw();
	bool press(int mouseX, int mouseY);
	void release();
	void computeSize();

	void onTimeAnimationFinished() override;
	void onAlphaAnimationFinished() override;

	std::string getLabel();
	void setLabel(std::string label);
	float getLeft();
	float getRight();
	float getTop();
	float getBottom();
	float getWidth();
	float getHeight();
	
	static float maxWidth, maxHeight;

	static ofEvent<key> staticKeyPressed;
	ofEvent<key> keyPressed;

	//indique si la touche diffuse un événement lorsque l'on clique dessus
	bool dispatchEvent;

	ofColor borderColor;
	ofColor backgroundColor;
	ofColor fontColor;

private:
	const int PADDING = 10;

	std::string label;

	ofRectangle bounds;
	bool sameSize;

	ofTrueTypeFont * keywordFont;

	bool pressed = false;
};