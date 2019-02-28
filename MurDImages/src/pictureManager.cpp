#include "pictureManager.h"

ofEvent<void> pictureManager::readyEvent = ofEvent<void>();

pictureManager::pictureManager()
{
	wordManager = keywordManager::getInstance();
	altasLoaded = false;
}

pictureManager::~pictureManager()
{
}

void pictureManager::setup()
{	
	//TODO: décommenter pour recréer les atlas
	//jLoader.createAtlas();

	//charge les données provenant du fichier JSON
	const bool dataLoaded = jLoader.loadData(&mesImages);
	if (dataLoaded) {
		ofLogNotice("ofApp::setup") << "Images chargées : " << mesImages.size();
		for (auto& img : mesImages)
		{
			//ajoute les mots-clés de chaque image au manager de mots-clés
			wordManager->extractKeywordsFromImage(&img);
			img.position.x = ofRandomWidth();
			img.position.y = ofRandomHeight();
		}
	}
	else
	{
		ofLogError("Erreur lors du chargement des données");
	}

	//chargement des atlas
	
	ofAddListener(atlasCreator.eventAllAtlasesLoaded, this, &pictureManager::onAtlasesLoaded);
	atlasCreator.loadAtlasesFromDisk(GL_RGBA, //internal format
		"textureCache", //dir
		"png", //image format
		true, //gen mipmaps
		-0.9 //mipmap bias
	);
}

void pictureManager::update() {
	jLoader.update();
}


void pictureManager::onAtlasesLoaded(bool &) {
	ofRemoveListener(atlasCreator.eventAllAtlasesLoaded, this, &pictureManager::onAtlasesLoaded);
	atlasCreator.registerWithManager(atlasManager);
	altasLoaded = true;
	filesToDraw = atlasCreator.getAllImagePaths();

	ofNotifyEvent(readyEvent);
}