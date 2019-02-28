#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofLogNotice("ofApp::setup") << "Starting setup";
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	//TODO : peut-être nécessaire pour repasser des photos l'une devant l'autre
	//glEnable(GL_DEPTH_TEST);
	
	ofSetFullscreen(true);

	lastTimeClick = ofGetElapsedTimeMillis();

	//old OF default is 96 - but this results in fonts looking larger than in other programs.
	ofTrueTypeFont::setGlobalDpi(72);

	ofLogNotice("ofApp::setup") << ofGetWidth() << ";" << ofGetHeight() * 0.33;
	keywordsBoard.setDimension(ofGetWidth(), (ofGetHeight() * 0.33));
	ofAddListener(touchBoard::backToLettersEvent, this, &ofApp::backToLetters);
	ofAddListener(touchBoard::zoomOutEvent, this, &ofApp::zoomOut);
	ofAddListener(touchBoard::zoomInEvent, this, &ofApp::zoomIn);
	ofAddListener(touchBoard::centerEvent, this, &ofApp::onCenterEvent);

	timer = new easingValue<float>(0, 2);
	ofAddListener(timer->easingFinished, this, &ofApp::onTimer);
	
	// enable the Windows Touch Hook
	ofxWinTouchHook::EnableTouch();

	// add touch listeners
	ofAddListener(ofxWinTouchHook::touchDown, this, &ofApp::touchDown);
	ofAddListener(ofxWinTouchHook::touchMoved, this, &ofApp::touchMoved);
	ofAddListener(ofxWinTouchHook::touchUp, this, &ofApp::touchUp);

	wordManager = keywordManager::getInstance();

	pictureManager = pictureManager::getInstance();
	ofAddListener(pictureManager::readyEvent, this, &ofApp::onDataLoaded);
	pictureManager->setup();

	currentState = ofApp::STATE_LOADING;
}

void ofApp::setupProjector()
{
}

//--------------------------------------------------------------
void ofApp::update(){
	if (currentState != STATE_LOADING) {
		keywordsBoard.refresh();
		cloud.update();
		timer->update();
	}

	//if(isSingleClick && !dragging)
	if (isSingleClick)
	{
		uint64_t now = ofGetElapsedTimeMillis();
		if (now - lastTimeClick >= DOUBLE_CLICK_THRESHOLD)
		{
			isSingleClick = false;
			image * imageToDisplay;
			if(cloud.press(clickX, clickY, &imageToDisplay))
			{
				projector->setPictureToDisplay(imageToDisplay);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	if (currentState != STATE_LOADING) {
		ofBackgroundGradient(ofColor(150, 150, 150), ofColor(25, 25, 25));

		cloud.draw();
			
		keywordsBoard.draw();
	}
}

void ofApp::drawProjector(ofEventArgs& args)
{
}


/**
 * \brief Méthode appelée lorsque les données (json + atlas) ont été chargées.
 * Enregistré dans la méthode setup() : ofAddListener(pictureManager::readyEvent, this, &ofApp::onDataLoaded);
 */
void ofApp::onDataLoaded()
{
	ofLogNotice("ofApp::onDataLoaded");
	//on supprime le listener une fois les données chargées (il ne sert plus à rien)
	ofRemoveListener(pictureManager::readyEvent, this, &ofApp::onDataLoaded);

	ofAddListener(touchBoard::keywordSelectedEvent, this, &ofApp::keywordPressed);
	ofAddListener(touchBoard::keywordDeletedEvent, this, &ofApp::keywordDeleted);

	ofLogNotice("ofApp::setup") << "Total mots-clés " << wordManager->nKeywords;
	
	keywordsBoard.addAllKey(wordManager->getKeys());

	cloud.init();

	currentState = STATE_DISPLAY_ALPHA;	
}

//void ofApp::keywordPressed(key &key)
void ofApp::keywordPressed(string & word) {
	ofLogNotice("ofApp::keywordPressed") << word;
	switch (currentState)
	{
	case STATE_DISPLAY_ALPHA:
	{
			char letter = word[0];
			keywordsBoard.clear();
			keywordsBoard.addAllKey(wordManager->getValues(letter));
			currentState = STATE_DISPLAY_KEYWORDS;
	}
		break;
	case STATE_DISPLAY_KEYWORDS:
		cloud.addKeywordToFilter(word);
		wordManager->addWordToFilter(word);
		backToLetters();
		break;
	}
}

void ofApp::keywordDeleted(string & word)
{
	cloud.removeKeywordToFilter(word);
	wordManager->removeWordFromFilter(word);
	//if(currentState == STATE_DISPLAY_ALPHA)
	//{
		backToLetters();
	//}
}

void ofApp::backToLetters()
{
	keywordsBoard.clear();
	keywordsBoard.addAllKey(wordManager->getKeys());
	currentState = STATE_DISPLAY_ALPHA;
}

void ofApp::zoomIn()
{
	cloud.zoomIn();
}

void ofApp::zoomOut()
{
	cloud.zoomOut();
}

void ofApp::onCenterEvent()
{
	cloud.recenterCloud();
}

void ofApp::onTimer() {
	//ofLogNotice("ofApp::onTimer") << keywordsBoard.isVisible();
	if (!keywordsBoard.isVisible()) {
		keywordsBoard.show();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
	/*if(key == '-')
	{
		cloud.zoomOut();
	}*/
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){	
	//ofLogNotice("ofApp::mouseDragged") << dragging;
	if(!dragging)
	{
		//isSingleClick = false;
		dragging = true;
		previousX = x;
		previousY = y;
	}

	dx = x - previousX;
	dy = y - previousY; 
	cloud.mouseDragged(dx, dy);
	timer->setCurrentValue(0);

	previousX = x;
	previousY = y;	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	clickX = x;
	clickY = y;
	if(!keywordsBoard.mousePressed(x, y))
	{
		//ofLogNotice("ofApp::mousePressed") << "pressed!";
		if (keywordsBoard.isVisible()) {
			//ofLogNotice("ofApp::mousePressed") << "hide keyboard";
			keywordsBoard.hide();
		}
		timer->setCurrentValue(0);
		
		//TODO: déplacer dans mouseReleased ???
		isSingleClick = true;
		uint64_t now = ofGetElapsedTimeMillis();
		//ofLogNotice("ofApp::mousePressed") << (now - lastTimeClick);
		if(now - lastTimeClick < DOUBLE_CLICK_THRESHOLD)
		{
			isSingleClick = false;
			//ofLogNotice("ofApp::mousePressed") << "double clicked";
			cloud.zoomIn();
		}
		lastTimeClick = now;		
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	keywordsBoard.mouseReleased();

	//ofLogNotice("ofApp::mouseReleased") << dragging;
	if(dragging)
	{
		cloud.setSpeed(dx, dy);
	}
	timer->setNewValue(1);

	dragging = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
	//ofLogNotice("ofApp::mouseEntered");
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
	//ofLogNotice("ofApp::mouseExited");
}

void ofApp::touchDown(ofTouchEventArgs & touch) {
	ofLogNotice("touch") << "touch down: " << touch.x << ", " << touch.y << ", id: " << touch.id;
}

void ofApp::touchMoved(ofTouchEventArgs & touch) {
	ofLog() << "touch move: " << touch.x << ", " << touch.y << ", id: " << touch.id;
}

void ofApp::touchUp(ofTouchEventArgs & touch) {
	ofLog() << "touch up: " << touch.x << ", " << touch.y << ", id: " << touch.id;
}

void ofApp::touchDoubleTap(ofTouchEventArgs& touch)
{
	ofLog() << "touch double tap: " << touch.x << ", " << touch.y << ", id: " << touch.id;
}

void ofApp::touchCancelled(ofTouchEventArgs& touch)
{
	ofLog() << "touch cancelled: " << touch.x << ", " << touch.y << ", id: " << touch.id;
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	ofLogNotice("ofApp::windowResized") << w << ";" << h;
	keywordsBoard.setDimension(w, (h * 0.33));
	if (currentState != STATE_LOADING) {
		keywordsBoard.sort();
		cloud.windowResized(w, h);
	}
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
	ofLogNotice("ofApp::gotMessage") << msg.message;
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
