#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "keywordManager.h"
#include "key.h"
#include "touchBoard.h"
#include "pictureManager.h"
#include "pictureCloud.h"

#include "ofxWinTouchHook.h"
#include "projectorApp.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void setupProjector();
		void update(); 
		void draw();
		void drawProjector(ofEventArgs & args);

		void onDataLoaded();
		void keywordPressed(string & word);
		void keywordDeleted(string & word);

		void backToLetters();
		void zoomIn();
		void zoomOut();
		void onCenterEvent();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		// add the touch listeners
		void touchDown(ofTouchEventArgs& touch) override;
		void touchMoved(ofTouchEventArgs& touch) override;
		void touchUp(ofTouchEventArgs& touch) override;
		void touchDoubleTap(ofTouchEventArgs& touch) override;
		void touchCancelled(ofTouchEventArgs& touch) override;
	
		pictureManager * pictureManager;
		keywordManager * wordManager;
		pictureCloud cloud;
		touchBoard keywordsBoard;

		bool dragging = false;
		int previousX, previousY;
		int dx = 0, dy = 0;

		shared_ptr<projectorApp> projector;

private:
	void onTimer();

	static const int STATE_LOADING = 0;
	static const int STATE_DISPLAY_ALPHA = 1;
	static const int STATE_DISPLAY_KEYWORDS = 2;

	int currentState = STATE_LOADING;

	static const int DOUBLE_CLICK_THRESHOLD = 200;
	uint64_t lastTimeClick;
	int clickX, clickY;
	bool isSingleClick = false;

	easingValue<float> * timer;
	                                                                              
};
