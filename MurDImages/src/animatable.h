#pragma once

#include "ofLog.h"
#include "ofUtils.h"
#include "ofxEasing.h"
#include "ofEvents.h"
#include "easingValue.h"

class animatable {
public:
	animatable();
	~animatable();

	void update();
	void setAlpha(int origin, int target);
	void moveTo(int targetX, int targetY);
	void setPosition(int x, int y);
	void translate(int dx, int dy);

	// Méthodes abstraites
	virtual void onAlphaAnimationFinished() = 0;
	virtual void onTimeAnimationFinished() = 0;

	//Constantes
	const float DURATION_ALPHA = 0.2f;
	const float DURATION_POSITION = 0.5f;

	//Attributs
	easingValue<float> * x;
	easingValue<float> * y;
	easingValue<float> * backgroundAlpha;
	bool staticPosition;

	//bool positionEventFired = true;

};