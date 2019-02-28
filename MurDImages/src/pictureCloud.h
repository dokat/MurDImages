#pragma once
#include <vector>
#include "ofEvents.h"
#include "utils.h"
#include "image.h"
#include "pictureManager.h"
#include "easingValue.h"
#include "spring.h"

using namespace std;

class pictureCloud
{
public:
	pictureCloud();
	~pictureCloud();

	void init();
	void update();
	void draw();
	void drawPicture(image * img);
	void reset();
	bool press(int x, int y, image ** imageToDisplay);
	void addKeywordToFilter(string keyword);
	void removeKeywordToFilter(string keyword);

	void zoomIn();
	void zoomOut();
	void recenterCloud();

	////////////
	//Events
	void windowResized(int w, int h);
	void mouseDragged(int dx, int dy);
	void setSpeed(int dx, int dy);
	void onZoomOutFinished();
	void onRecenterFinished();	

	pictureManager * pictureManager;
	//contient la toutes les images
	vector<image*> allThePictures;
	//contient la liste des images que l'on peut afficher (en fonction des mots-clés sélectionnés)
	vector<image*> picturesToDisplay;
	
	vector<spring*> springs;
	
	image center;

	easingValue<float> * recenter;
	easingValue<float> * scale;
	easingValue<ofVec2f> * pos;

	vector<string> keywordsFilter;

	ofVec2f position;
	//TODO : voir pour utiliser easingValue
	ofVec2f velocity;
	float dampling = 0.1;

	const float ZOOM_FACTOR = 0.1;

	//TODO : doit être calculer par rapport à la position des images
	float minX = -1500;
	float minY = -1500;
	float maxX = 1500;
	float maxY = 1500;
};