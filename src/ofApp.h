#pragma once

#include "ofMain.h"
#include "appSettings.h"

#ifdef ANDROID_VERSION
    #include "ofxAndroid.h"
    #include "ofxAccelerometer.h"
#endif


#include "line.h"
#include "lineUtilities.h"
#include "ofxGui.h"
#include "ofxGifEncoder.h"
#include "menu.h"
#include "ofxXmlSettings.h"
#include "introScreen.h"


#ifdef ANDROID_VERSION
	class ofApp : public ofxAndroidApp{
#else
	class ofApp : public ofBaseApp{
#endif

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
        //--------------------------- android specific stuff
        void touchDown(int x, int y, int id);
        void touchMoved(int x, int y, int id);
        void touchUp(int x, int y, int id);
        void touchDoubleTap(int x, int y, int id);
        //void touchCancelled(int x, int y, int id);
           
        void pause();
        void stop();
        void resume();
        void reloadTextures();
        void unloadTextures();
    
        bool backPressed();
        void okPressed();
        void cancelPressed();
    
        ofPoint normalizedSmoothed;
        ofVec3f accel, normAccel;
    
    
        lineWithInfo line;
        vector < lineWithInfo > lines;

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

        //ofxPanel gui;

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
    
        
    
        introScreen IS;
    
    
        void exportDrawingsToFile();

        bool bIsThisMouseEventInMenu;

        ofxGifEncoder gifEncoder;
        ofPixels subscreenPixels;
        unsigned char * data;

        bool bPaused;

        menu menuPanel;

    
        ofImage experimentImg;
        ofImage recordingImg;
        ofImage processingImg;
        ofImage closeImg;

    
        int screenWidth, screenHeight;
    
#ifndef ANDROID_VERSION
        ofFbo testFbo;
#endif
    
        ofRectangle closeImageRect;

        bool bFirstPressOfApp;

    
    
		
};
