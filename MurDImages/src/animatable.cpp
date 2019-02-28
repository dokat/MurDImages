#include "animatable.h"

animatable::animatable()
{
	//ofLogNotice("animatable::animatable");
	staticPosition = false;
	x = new easingValue<float>(0, DURATION_POSITION);
	//ofAddListener(x->easingFinished, this, &animatable::onAlphaAnimationFinished);
	y = new easingValue<float>(0, DURATION_POSITION);
	//ofAddListener(y->easingFinished, this, &animatable::onAlphaAnimationFinished);
	backgroundAlpha = new easingValue<float>(0, DURATION_ALPHA);
	ofAddListener(backgroundAlpha->easingFinished, this, &animatable::onAlphaAnimationFinished);
}

animatable::~animatable()
{
}

void animatable::update()
{
	if (!staticPosition) {
		x->update();
		y->update();
	}
	backgroundAlpha->update();
}

void animatable::setAlpha(int origin, int target) 
{
	ofLogNotice("animatable::setAlpha") << origin << ", " << target;
	backgroundAlpha->setCurrentValue(origin);
	backgroundAlpha->setNewValue(target);
}

void animatable::translate(int dx, int dy)
{
	moveTo(x->getValue() + dx, y->getValue() + dy);
}

/**
 * D�place l'�l�ment � la position donn�e (newX, newY) (un �v�nement est lanc� � la fin)
 */
void animatable::moveTo(int newX, int newY)
{
	x->setNewValue(newX);
	y->setNewValue(newY);
}

/**
 * Place l'�l�ment � la position donn�e (PAS d'�v�nement lanc� � la fin)
 */
void animatable::setPosition(const int newX, const int newY)
{
	x->setCurrentValue(newX);
	y->setCurrentValue(newY);
}
