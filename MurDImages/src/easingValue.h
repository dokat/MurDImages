#pragma once
#include "ofEvent.h"
#include "ofUtils.h"
#include "ofxEasing.h"
#include "ofEventUtils.h"

template<class T>
class easingValue
{
private:
	T currentValue;
	T originValue;
	T targetValue;
	float iniTime;
	bool eventFired;

public:
	ofEvent<void> easingFinished;

	easingValue(T value, float duration)
	{
		iniTime = ofGetElapsedTimef();
		originValue = value;
		targetValue = value;
		currentValue = value;
		this->duration = duration;
		eventFired = true;
	}
	
	T getValue()
	{
		return currentValue;
	}

	T getTargetValue()
	{
		return targetValue;
	}
	
	void setNewValue(T newValue)
	{
		//ofLogNotice("setNewValue") << newValue << " (current: " << currentValue << ")";
		originValue = currentValue;
		targetValue = newValue;
		iniTime = ofGetElapsedTimef();
		eventFired = false;
	}

	void setCurrentValue(T value)
	{
		originValue = value;
		targetValue = value;
		currentValue = value;
		iniTime = ofGetElapsedTimef();
		eventFired = true;
	}

	/**
	 * Il ne faut surtout pas oublier d'appeler méthode pour faire la mise à jour de la valeur.
	 */
	void update()
	{
		const float now = ofGetElapsedTimef();
		const float endTime = iniTime + duration;
		
		if (!eventFired)
		{
			currentValue = ofxeasing::map_clamp(now, iniTime, endTime, originValue, targetValue, &ofxeasing::cubic::easeOut);
			if (currentValue == targetValue)
			{
				eventFired = true;
				ofNotifyEvent(easingFinished);
			}
		}
	}

	float duration;
};
