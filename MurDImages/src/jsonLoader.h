#pragma once
#include <codecvt>
#include <locale>
#include <vector>
#include <utf8.h>
#include "ofxJSON.h"
#include "image.h"
#include "utils.h"
#include "ofxDownloadCentral.h"
#include "TextureAtlasCreator.h"

class jsonLoader
{
public:
	jsonLoader();
	~jsonLoader();

	bool loadData(std::vector<image> * values);
	void update();

	void createAtlas();

private:
	ofxJSONElement loadJsonFromServer();

	ofxJSONElement loadJsonFromDisk();

	void downloadFinished(ofxBatchDownloaderReport &report);

	void onAtlasCreationFinished(bool & arg);

	//Attributs
	ofxDownloadCentral downloader;

	TextureAtlasCreator atlasCreator;
};

