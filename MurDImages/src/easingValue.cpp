#include "easingValue.h"

/*template <class T>
easingValue<T>::easingValue(T value, float duration)
{
	iniTime = ofGetElapsedTimef();
	originValue = value;
	targetValue = value;
	currentValue = value;
	this->duration = duration;
}

template <class T>
T easingValue<T>::getValue()
{
	return currentValue;
}


template <class T>
void easingValue<T>::setNewValue(T newValue)
{
	targetValue = newValue;
	iniTime = ofGetElapsedTimef();
}

template <class T>
void easingValue<T>::update()
{
	const float now = ofGetElapsedTimef();
	const float endTime = iniTime + duration;

	currentValue = ofxeasing::map_clamp(now, iniTime, endTime, originValue, targetValue, &ofxeasing::cubic::easeIn);
}
*/
