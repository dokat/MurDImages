#include "image.h"

image::image()
{
	this->position.x = 0;
	this->position.y = 0;
}


image::~image()
{
}

void image::setRepresentations(string value)
{
	int positionColon = value.find_first_of(':'); // position des 2 points (:)
	int positionBracketOpen = value.find_first_of('(');
	int positionBracketClose = value.find_first_of(')');

	representationsUtf8 = ofSplitString(value.substr(positionBracketOpen + 1, (positionBracketClose - positionBracketOpen - 1)), ",", true, true);
	representationsUtf8.push_back(value.substr(positionColon + 1, (positionBracketOpen - positionColon - 2)));
	for (auto &word : representationsUtf8) {
		representations.push_back(utils::utf8ToString(word));
	}
}

void image::setSize(int width, int height)
{
	size = ofVec2f(width, height);
	radius = sqrt(width * width + height * height) + 20;
}

vector<string> image::getRepresentations() {
	return representationsUtf8;
}

string image::getInscriptions()
{
	ofStringReplace(inscription, "\r\n", ":");
	vector<string> detail = ofSplitString(inscription, ":", false, true);
	ofLogNotice("image::getInscriptions") << filename << " : " << ofToString(detail) << " -- " << detail.size() ;
	return detail.size() >= 6 ? detail[2] + ", " + detail[5] : "";
}

ofVec2f image::getCenter()
{
	return position + (size / 2);
}


void image::setUrl(string url)
{
	this->url = url;

	//extrait le nom du fichier à partir de l'URL
	filename = url.substr(url.find_last_of('/') + 1);


}

string image::getUrl()
{
	return url;
}


string image::getFilename()
{
	return "images\\" + filename;
}

bool image::contains(vector<string> keywords)
{
	bool result = true;	
	vector<string>::iterator it;
	size_t i = 0;
	while (result && i < keywords.size()) {
		it = find(representations.begin(), representations.end(), keywords[i]);
		result = it != representations.end();
		i++;
	}
	return result;
}

void image::attract(vector<image>& nodes)
{
	for (auto &node : nodes) {
		if (&node != this) {
			attract(&node);
		}
	}
}

void image::attract(vector<image*> * nodes)
{
	for (auto node : *nodes) {
		if (node != this) {
			attract(node);
		}
	}
}

void image::attract(image * otherNode)
{
	float dist = this->position.distance(otherNode->position);
	if (dist > 0 && dist < radius) {
		float s = pow(dist / radius, 1 / ramp);
		float f = s * 9 * strength * (1 / (s + 1) + ((s - 3) / 4)) / dist;

		otherNode->velocity += (this->position - otherNode->position) * f;
	}
}

void image::update()
{
	velocity.limit(maxVelocity);

	this->position += this->velocity;

	/*if (this->position.x > limits.x)
	{
		//position.x = limits.x - (position.x - limits.x);
		this->velocity.x = -this->velocity.x;
	}
	if (position.x < 0)
	{
		//position.x = -position.x;
		this->velocity.x = -this->velocity.x;
	}
	if (this->position.y > limits.y)
	{
		//position.y = limits.y - (position.y - limits.y);
		this->velocity.y = -this->velocity.y;
	}
	if (position.y < 0)
	{
		//position.y = -position.y;
		this->velocity.y = -this->velocity.y;
	}*/

	velocity *= (1 - dampling);
}

string image::toString()
{
	return "Image [url=" + url + "]";
}

void image::zoom(float zoom)
{
	radius *= zoom;
}
