#include "spring.h"

spring::spring(image * from, image * to)
{
	fromNode = from;
	toNode = to;
}

void spring::update()
{
	ofVec2f diff = toNode->position - fromNode->position;
	diff.normalize();
	diff *= length;
	ofVec2f target = fromNode->position + diff;

	ofVec2f force = target - toNode->position;
	force *= 1;
	force *= stiffness;
	force *= (1 - damping);

	toNode->velocity += force;
	fromNode->velocity += (force * -1);
}

