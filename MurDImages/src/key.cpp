#include "key.h"

float key::maxWidth = 0;
float key::maxHeight = 0;

ofEvent<key> key::staticKeyPressed = ofEvent<key>();

key::key(std::string label, ofTrueTypeFont * font, int x, int y, bool sameSize) : animatable()
{
	//ofLogNotice("key::key") << label;
	this->sameSize = sameSize;
	this->keywordFont = font;

	backgroundAlpha->setCurrentValue(75);
	setPosition(x, y);

	fontColor.set(50, 50, 50, 255);
	borderColor.set(255);
	backgroundColor.set(150);

	dispatchEvent = true;

	setLabel(label);
}

key::~key()
{
}

void key::computeSize()
{
	bounds = keywordFont->getStringBoundingBox(label, 0, 0);
	//!! bound.x est toujours égal à 0 !!
	//Quoiqu'il arrive, on veut toujours que les touches aies la même hauteur.
	key::maxHeight = std::max(bounds.height, key::maxHeight);

	if (sameSize) {
		key::maxWidth = std::max(bounds.width, maxWidth);
	}
}

std::string key::getLabel()
{
	return label;
}

void key::setLabel(std::string label)
{
	this->label = label;
	computeSize();
}

float key::getLeft() {
	return x->getValue() - PADDING;
}

float key::getRight() {
	return x->getValue() + PADDING + (sameSize ? key::maxWidth : bounds.width);
}

float key::getTop() {
	return y->getValue() - PADDING;
}

float key::getBottom() {
	return y->getValue() + PADDING + key::maxHeight;
}

float key::getWidth() {
	return getRight() - getLeft();
}

float key::getHeight() {
	return  getBottom() - getTop();
}

void key::reset() {
	key::maxWidth = 0;
	key::maxHeight = 0;
}

void key::draw() {
	ofFill();
	ofSetColor(backgroundColor, backgroundAlpha->getValue());
	ofDrawRectRounded(x->getValue(), getTop(), getWidth(), getHeight(), 10);
	ofSetColor(borderColor);
	ofNoFill();
	ofDrawRectRounded(x->getValue(), getTop(), getWidth(), getHeight(), 10);
	ofSetColor(fontColor);
	keywordFont->drawString(label, x->getValue()+PADDING, y->getValue() + getHeight() / 2);
	ofSetColor(ofColor::white);
}

bool key::press(int mouseX, int mouseY)
{	
	pressed = mouseX > getLeft() && mouseX < getRight() && mouseY > getTop() && mouseY < getBottom();
	return pressed;
}

void key::release()
{
	if (pressed) {
		pressed = false;
		setAlpha(255, 75);
	}
}

void key::onTimeAnimationFinished() {

}

void key::onAlphaAnimationFinished() {
	ofLogNotice("key::onAlphaAnimationFinished") << dispatchEvent;
	if (dispatchEvent) 
	{
		ofNotifyEvent(staticKeyPressed, *this);		
	} else
	{
		ofNotifyEvent(keyPressed, *this);
	}
}