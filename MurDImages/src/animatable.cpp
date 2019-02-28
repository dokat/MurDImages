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
 * Déplace l'élément à la position donnée (newX, newY) (un événement est lancé à la fin)
 */
void animatable::moveTo(int newX, int newY)
{
	x->setNewValue(newX);
	y->setNewValue(newY);
}

/**
 * Place l'élément à la position donnée (PAS d'événement lancé à la fin)
 */
void animatable::setPosition(const int newX, const int newY)
{
	x->setCurrentValue(newX);
	y->setCurrentValue(newY);
}
