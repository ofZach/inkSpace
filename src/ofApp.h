#pragma once

#include "ofMain.h"
#include "ofxAndroid.h"
#include "ofxAccelerometer.h"
#include "line.h"
#include "lineUtilities.h"
#include "ofxGui.h"
#include "ofxGifEncoder.h"





class ofApp : public ofxAndroidApp{
	
	public:
		
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void windowResized(int w, int h);

		void touchDown(int x, int y, int id);
		void touchMoved(int x, int y, int id);
		void touchUp(int x, int y, int id);
		void touchDoubleTap(int x, int y, int id);
		void touchCancelled(int x, int y, int id);
		void swipe(ofxAndroidSwipeDir swipeDir, int id);

		void pause();
		void stop();
		void resume();
		void reloadTextures();

		bool backPressed();
		void okPressed();
		void cancelPressed();

		ofPoint normalizedSmoothed;
		ofTrueTypeFont font;
		ofVec3f accel, normAccel;
		string messages[3];
		lineWithInfo line;
        vector < lineWithInfo > lines;
       // rhondaLineRenderer RLR;
        ofVbo * vbo;
        int nPointsLastFrame;
        ofShader shader;

        int lastNumPts;

        ofImage ink;
        bool bNeedToReallocateOpengl;
        int nFramesToReallocate;

        //------------------------------------------ gui

    	bool bHide;
    
        ofParameter < float > accelSmooth;
        ofParameter < float > rotateAmount;
        ofParameter < float > rotateDamping;
        ofParameter < float > transAmount;
        ofParameter < float > transDamping;
        ofParameter < float > minWidth;
        ofParameter < float > maxWidth;
    
    
//    
//        // z scaling
//    
//    
//    	ofParameter<float> radius;
//    	ofParameter<ofColor> color;
//    	ofParameter<ofVec2f> center;
//    	ofParameter<int> circleResolution;
//    	ofParameter<bool> filled;
//    	ofxButton twoCircles;
//    	ofxButton ringButton;
//    	ofParameter<string> screenSize;
    	ofxPanel gui;


    	 void onGifSaved(string & fileName);
    	 bool bGifSaved;
    	 string gifPath;



//06-13 06:33:21.320: I/(16068): /storage/emulated/0/Android/data/cc.openframeworks.accelDraw/files/test2015-06-13-06-33-21-313.gif


    	ofImage camImg;
    	ofImage infoImg;
    	ofImage lightningImg;
    	ofImage arrowImg;

    	bool bLightningMode;
    	bool bRecordingMode;
    	int recordingStartTime;
    	bool bInfoMode;
    	float triPct;
    	float triangleChangeEnergy;


    	void handleMenuPress(ofPoint pt, bool bDrag);

    	bool bMenuOpen;
    	float menuOpenPct;
    	bool bIsThisMouseEventInMenu;


    	 ofxGifEncoder gifEncoder;

    	ofPixels subscreenPixels;
    	ofFbo subscreen;
    	ofFbo wholeScreen;

    	ofTexture screen;
    	ofFbo menu;
    	unsigned char * data;

};
