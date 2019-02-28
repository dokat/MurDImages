#include "jsonLoader.h"

#define DEFAULT_URL "http://datarmor.cotesdarmor.fr/dataserver/cg22/data/22278_MuseeFondsBailly?$format=json&$top=1000"

#define JSON_FILENAME "data.json"

//TODO : automatiser le chargement des données et la création des atlas
//TODO : faire un retour visuel sur la progression du chargement

jsonLoader::jsonLoader()
{
	downloader.setNeedsChecksumMatchToSkipDownload(true);
	downloader.setIdleTimeAfterEachDownload(0.2);
	downloader.setVerbose(true);
	downloader.setMaxConcurrentDownloads(3);
}


jsonLoader::~jsonLoader()
{
}

bool jsonLoader::loadData(std::vector<image> * values)
{
	bool dataLoaded = false;

	ofxJSONElement json = loadJsonFromDisk();

	if (json == Json::Value::null)
	{
		json = loadJsonFromServer();
	}

	if (json != Json::Value::null)
	{
		int nbElements = json["d"]["results"].size();

		for (Json::ArrayIndex i = 0 ; i < nbElements ; ++i) {			
			string description = utils::utf8ToString(json["d"]["results"][i]["Description"].asString());
			//std::string representations = utils::utf8ToString(json["d"]["results"][i]["Reprsentations"].asString());
			string representations = json["d"]["results"][i]["Reprsentations"].asString();
			string mesures = utils::utf8ToString(json["d"]["results"][i]["Mesures"].asString());
			string techniques = utils::utf8ToString(json["d"]["results"][i]["Matriauxettechniques"].asString());
			string id = utils::utf8ToString(json["d"]["results"][i]["id__"].asString());
			string imageURL = utils::utf8ToString(json["d"]["results"][i]["Image"].asString());
			string numInventaire = utils::utf8ToString(json["d"]["results"][i]["intitul"].asString());
			//string inscription = utils::utf8ToString(json["d"]["results"][i]["Inscriptions"].asString());
			string inscription = json["d"]["results"][i]["Inscriptions"].asString();

			if (imageURL.size() > 0) {
				image newImage;
				newImage.id = imageURL;
				newImage.mesures = mesures;
				newImage.techniques = techniques;
				newImage.setUrl(imageURL);
				newImage.setRepresentations(representations);
				newImage.description = description;
				newImage.numInventaire = numInventaire;
				newImage.inscription = inscription;

				values->push_back(newImage);
			}
		}

		dataLoaded = true;
	}
	
	return dataLoaded;
}

void jsonLoader::update()
{
	downloader.update();
}

ofxJSONElement jsonLoader::loadJsonFromServer()
{
	ofLogNotice("jsonLoader::loadJsonFromServer");
	//TODO: créer fichier de configuration pour l'URL

	ofxJSONElement json;
	if (json.open(DEFAULT_URL))
	{
		//Sauvegarde le json sur le disque
		if (!json.save(JSON_FILENAME, true))
		{
			ofLogNotice("jsonLoader::loadJsonFromServer") << "data.json written unsuccessfully.";
		}
		else
		{
			ofLogNotice("jsonLoader::loadJsonFromServer") << "data.json written successfully.";
		}

		//Ajoute les photos à télécharger dans la file de téléchargement
		int nbElements = json["d"]["results"].size();
		std::vector<std::string> urlsToDownload;

		for (Json::ArrayIndex i = 0; i < nbElements; ++i) {
			std::string imageURL = json["d"]["results"][i]["Image"].asString();
			if (!imageURL.empty()) {
				urlsToDownload.push_back(imageURL);
				ofLogNotice("jsonLoader::loadJsonFromServer") << "Adding " << imageURL << " to download list";
			}
		}
		downloader.downloadResources(urlsToDownload,	//list of urls to download
			this,							//who will be notified
			&jsonLoader::downloadFinished,	//callback method
			"images"					//destination folder
		);
		downloader.startDownloading();

	}
	else
	{
		ofLogError("Impossible de charger les données du serveur.");
	}

	return json;
}

ofxJSONElement jsonLoader::loadJsonFromDisk()
{
	//TODO: chargement des données depuis le disque
	ofLogNotice("jsonLoader::loadJsonFromDisk") << "Début du chargement des données locales...";
	ofxJSONElement json;
	const bool parsingSuccessful = json.openLocal(JSON_FILENAME);
	if (parsingSuccessful)
	{
		ofLogNotice("jsonLoader::loadJsonFromDisk") << "... Données locales chargées";
	}
	else
	{
		ofLogNotice("jsonLoader::loadJsonFromDisk") << "... Erreur lors du chargement des données locales";
	}
	
	return json;
}

void jsonLoader::downloadFinished(ofxBatchDownloaderReport &report)
{
	ofLogNotice("jsonLoader::downloadFinished") << "download Finished!";
	ofLogNotice("jsonLoader::downloadFinished") << report.successfulDownloads.size() << " successful downloads, " << report.failedDownloads.size() << " failed downloads.";

	if (report.failedDownloads.empty()) {
		ofLogError("jsonLoader::downloadFinished") << "these downloads failed: ";
		for (const auto& failedDownload : report.failedDownloads)
		{
			ofLogError("jsonLoader::downloadFinished") << " - " << failedDownload;
		}
	}
}

void jsonLoader::createAtlas() {
	ofAddListener(atlasCreator.eventAtlasCreationFinished, this, &jsonLoader::onAtlasCreationFinished);

	vector<string> fileList;
	ofDirectory d;
	d.allowExt("jpg");
	d.listDir("images");
	d.sort();
	ofLogNotice("jsonLoader::createAtlas") << d.size() << " images => atlas";
	for (int i = 0; i < d.size(); i++) {
		fileList.push_back(d.getPath(i));
	}
	//create as many atlases as required to fit all those images in atlases of 4096 x 4096
	//each image being 256 at the most.
	atlasCreator.createAtlases(fileList,
		4096, 	//fbo/atlas size
		GL_RGB, //internal format
		ofVec2f(400, 400), //maxItemSideSize
		2.0,	//padding
		true, //mipmaps
		-0.9 //mipmap bias
	);
}

void jsonLoader::onAtlasCreationFinished(bool & arg) {
	//once atlases are created, save them to disk, so that we can load them next time
	//without having to recreate them
	atlasCreator.saveToDisk("textureCache", "png"); //save in a folder named "textureCache", in png format
	ofLogNotice("jsonLoader::onAtlasCreationFinished") << arg;
}