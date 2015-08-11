#pragma once

#include "ofMain.h"
#include "ofxAndroid.h"
#include "ofxAccelerometer.h"
#include "line.h"
#include "lineUtilities.h"
#include "ofxGui.h"
#include "ofxGifEncoder.h"
#include "menu.h"
#include "ofxXmlSettings.h"


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

//		void scaleBegin( float scale);
//		void scale( float scale);
//		void scaleEnd( float scale);


		void pause();
		void stop();
		void resume();
		void reloadTextures();
		void unloadTextures();

		bool backPressed();
		void okPressed();
		void cancelPressed();

		ofPoint normalizedSmoothed;
		ofTrueTypeFont font;
		ofVec3f accel, normAccel;
		string messages[3];
		lineWithInfo line;
        vector < lineWithInfo > lines;


        float introScreenEnergy;
        float introScreenEnergyTarget;
        vector < lineWithInfo > linesForIntroScreen;


        int drawingStartTime;
        bool bScaling;
        ofPoint fingerPos[2]; // for scaling!
        float fingerDiff;
        float scalePct;



        int nPointsLastFrame;
        

        int lastNumPts;


        //bool bNeedToReallocateOpengl;
        //int nFramesToReallocate;

        //------------------------------------------ gui

    	bool bHide;
    
        ofParameter < float > accelSmoothing;
        ofParameter < float > rotateAmount;
        ofParameter < float > rotateDamping;
        ofParameter < float > transAmount;
        ofParameter < float > transDamping;
        ofParameter < float > minWidth;
        ofParameter < float > maxWidth;


        ofParameter < float > rotateAmountInfo;
        ofParameter < float > rotateDampingInfo;
        ofParameter < float > transAmountInfo;
        ofParameter < float > transDampingInfo;



    	ofxPanel gui;


    	void onGifSaved(string & fileName);
    	bool bGifSaved;
    	string gifPath;


        //---------------------------------------------------------------------------------
        // images will get reloaded when the app context is destroyed, but I am not sure 
        // about FBOs, VBOs, and shaders, so I will reallocate them
    	
    	ofImage ink;
    	ofImage grey;

        //---------------------------------------------------------------------------------
        // things to reallocate when the opengl context resumes (when the app is paused, opengl context dissapears)
        ofVbo * vbo;
        ofShader * shader;
        ofFbo * subscreen;
        ofFbo * wholeScreen;
        ofTexture * screen;

        int gifWidth;
        int gifHeight;
        int lastGifRecordTime;
        int gifRecordRate;



    	bool bLightningMode;
    	bool bRecordingMode;
    	bool bRecordingProcessing;
    	int recordingStartTime;
    	float recordPct;
    	bool bInfoMode;

    	void setupTitle();
    	void drawTitle();
    	ofImage title;
    	vector < ofPoint > titlePts;

    	void exportDrawingsToFile();
    	

    	bool bIsThisMouseEventInMenu;
        
    	ofxGifEncoder gifEncoder;
    	ofPixels subscreenPixels;
    	unsigned char * data;

    	bool bPaused;

    	menu menuPanel;

    	ofDirectory infoScreenDir;

    	ofImage experimentImg;
    	ofImage recordingImg;
    	ofImage processingImg;


};
