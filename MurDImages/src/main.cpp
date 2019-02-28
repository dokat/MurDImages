#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"
#include "projectorApp.h"

//========================================================================
int main( ){
	//ofSetupOpenGL(1024,768, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	//ofRunApp(new ofApp());

	/*ofGLFWWindowSettings mainSettings;
	mainSettings.setSize(1024, 768);
	//mainSettings.windowMode = OF_FULLSCREEN;
	mainSettings.setPosition(ofVec2f(100, 100));
	mainSettings.monitor = 0;
	auto mainWindow = ofCreateWindow(mainSettings);
	shared_ptr<ofApp> mainApp(new ofApp);
	
	ofGLFWWindowSettings projectorSettings;
	projectorSettings.setPosition(ofVec2f(1920, 0));
	projectorSettings.setSize(1680, 1050);
	projectorSettings.resizable = false;
	projectorSettings.shareContextWith = mainWindow;
	projectorSettings.monitor = 1;
	auto projectorWindow = ofCreateWindow(projectorSettings);

	
	mainApp->setupProjector();
	ofAddListener(projectorWindow->events().draw, mainApp.get(), &ofApp::drawProjector);

	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();*/

	ofGLFWWindowSettings settings;
	//settings.windowMode = OF_FULLSCREEN;
	settings.setSize(1024, 768);
	settings.monitor = 0;
	settings.setPosition(ofVec2f(100,100));
	auto mainWindow = ofCreateWindow(settings);


	ofVec2f position, dimension;
	utils::getProjectorPositionAndSize(&position, &dimension);

	settings.setPosition(position);
	settings.monitor = 1;
	settings.setSize(dimension.x, dimension.y);
	settings.shareContextWith = mainWindow;
	auto projectorWindow = ofCreateWindow(settings);

	shared_ptr<ofApp> mainApp(new ofApp);
	shared_ptr<projectorApp> projApp(new projectorApp);
	
	mainApp->projector = projApp;

	ofRunApp(projectorWindow, projApp);
	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();

}
