#pragma once
#include "ofVec2f.h"
#include "image.h"

class spring
{
public:
	spring(image * from, image * to);

	void update();

	image * fromNode;
	image * toNode;

	float length = 10;
	float stiffness = 0.9;
	float damping = 0.5;

};