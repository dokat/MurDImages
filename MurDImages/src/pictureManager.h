#pragma once
#include <vector>
#include "ofEvent.h"
#include "image.h"
#include "singleton.h"
#include "jsonLoader.h"
#include "keywordManager.h"
#include "TextureAtlasDrawer.h"
#include "TextureAtlasCreator.h"

using namespace std;

class pictureManager : public singleton<pictureManager>
{
	friend class singleton<pictureManager>;

private:
	pictureManager();
	~pictureManager();

public:
	void setup();
	void update();
	void onAtlasesLoaded(bool &);

	static ofEvent<void> readyEvent;

	TextureAtlasCreator atlasCreator;
	TextureAtlasDrawer atlasManager;
	bool altasLoaded;

	vector<string> filesToDraw;
	vector<image> mesImages;
	jsonLoader jLoader;

	keywordManager * wordManager;

	static const int ATLAS_PICTURE_SIZE = 400;
};