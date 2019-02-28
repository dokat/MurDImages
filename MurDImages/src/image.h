#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "ofUtils.h"
#include "ofVec2f.h"
#include "utils.h"

using namespace std;

class image //: public node
{

private:
	string filename;
	string url;
	vector<string> representations;
	vector<string> representationsUtf8;

public:
	image();
	~image();
	void setUrl(string url);
	string getUrl();
	vector<string> getRepresentations();
	void setRepresentations(string representations);
	string getInscriptions();

	ofVec2f getCenter();	
	void setSize(int width, int height);
	string getFilename();
	bool contains(vector<string> keywords);
	string toString();
	void zoom(float zoom);

	void attract(vector<image>& nodes);
	void attract(vector<image*> * nodes);
	void attract(image * otherNode);
	void update();

	string description;	
	string mesures;
	string techniques;	
	string id;
	string numInventaire;
	string inscription;

	ofVec2f position;
	ofVec2f velocity;
	ofVec2f size;

	float maxVelocity = 2.f;
	float ramp = 1.f;
	float strength = -5.f;
	float dampling = 0.01;
	float radius = 100.f;
};

