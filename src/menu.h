
#pragma once


#include "ofMain.h"



class menu {

public:

    ofImage camImg;
    ofImage infoImg;
    ofImage lightningImg;
    ofImage arrowImg;
    ofImage trashImg;

    ofImage brushImg;
    ofImage brushImgInner;
    ofImage brushImgMix;

    void mixBrushImage();

    ofFbo * menuFbo;
    bool bMenuOpen;
    float menuOpenPct;

    int lastMenuTouchTime;
    ofRectangle mainRectangle;

    ofRectangle getBounds(){
    	return mainRectangle;
    }

	float triPct;
    	float triangleChangeEnergy;


    void setup();
    void update();
    void draw();
    void handleMenuPress(ofPoint pt, bool bDrag);
    void openglClosed();		// tear down and rebuild FBO
    void openglReOpened();

       ofRectangle camRect;//(50-32, 75-32, 64,64);
       ofRectangle infoRect;//(250-32, 75-32, 64,64);
       ofRectangle lightningRect;//(450-32, 75-32, 64,64);
       ofRectangle triangleRect;//(650-32, 75-32, 920-650, 64);
       ofRectangle arrowRect;//(1050-32,75-32, 64, 64);
       ofRectangle trashRect;
       ofRectangle fboRect;


};

