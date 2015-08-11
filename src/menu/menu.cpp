#include "menu.h"
#include "ofApp.h"



void menu::mixBrushImage(){

	unsigned char * pixA = brushImg.getPixels();
	unsigned char * pixB = brushImgInner.getPixels();
	unsigned char * pixC = brushImgMix.getPixels();

	int w = brushImg.getWidth();
	int h = brushImg.getHeight();
	int whichW = w * (1-triPct);

	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			if (i < whichW){
				pixC[(j * w + i)*4 + 0] = pixA[(j * w + i)*4 + 0];
				pixC[(j * w + i)*4 + 1] = pixA[(j * w + i)*4 + 1];
				pixC[(j * w + i)*4 + 2] = pixA[(j * w + i)*4 + 2];
				pixC[(j * w + i)*4 + 3] = pixA[(j * w + i)*4 + 3];
			} else {
				pixC[(j * w + i)*4 + 0] = pixB[(j * w + i)*4 + 0];
				pixC[(j * w + i)*4 + 1] = pixB[(j * w + i)*4 + 1];
				pixC[(j * w + i)*4 + 2] = pixB[(j * w + i)*4 + 2];
				pixC[(j * w + i)*4 + 3] = pixB[(j * w + i)*4 + 3];
			}
		}
	}

	brushImgMix.update();

}


void menu::setup(){



    menuFbo = new ofFbo();

    
    scale = 1.0;

    lastMenuTouchTime = 0;

    camImg.loadImage("menu/cam.png");
    infoImg.loadImage("menu/info.png");
    lightningImg.loadImage("menu/lightning.png");
    arrowImg.loadImage("menu/menu.png");
    trashImg.loadImage("menu/trash.png");
    brushImg.loadImage("menu/brushA.png");
    brushImgInner.loadImage("menu/brushB.png");

    brushImgMix.allocate(brushImg.getWidth(), brushImg.getHeight(), OF_IMAGE_COLOR_ALPHA);


    bMenuOpen = false;
    menuOpenPct = 0;
    triPct = 0.5;
    triangleChangeEnergy = 0.0;


     camRect.set(50-32, 75-32, 64,64);
     infoRect.set(200-32, 75-32, 64,64);
     lightningRect.set(350-32, 75-32, 64,64);
     triangleRect.set(500-32, 75-32, 200, 64);
     arrowRect.set(900-32,75-32, 64, 64);
     trashRect.set(750-32, 75-32, 64, 64);
     fboRect.set(0,0,1050,150);
     menuFbo->allocate(fboRect.width, fboRect.height);

     mixBrushImage();

}

void menu::update(){
    
    
    if (bMenuOpen){
        menuOpenPct = 0.9f * menuOpenPct + 0.1f * 1.0;
        int timeOpenSinceTouch = ofGetElapsedTimeMillis() - lastMenuTouchTime;
        //ofLog() << "timeOpenSinceTouch " << timeOpenSinceTouch;
        if (timeOpenSinceTouch > 6000){
            bMenuOpen = false;
        }
    } else {
        menuOpenPct = 0.93f * menuOpenPct + 0.07f * 0.0;
    }

    mainRectangle.set((-menuFbo->getWidth()*scale + 100) * (1-menuOpenPct),0, menuFbo->getWidth()*scale, menuFbo->getHeight()*scale);
    
}

void menu::draw(){
    
	ofApp * app= (ofApp*)ofGetAppPtr();

  

	menuFbo->begin();

        ofClear(0,0,0,180 * menuOpenPct);
        ofSetColor(ofColor::blueSteel);

        camImg.draw(camRect);
        infoImg.draw(infoRect);

        if (app->bLightningMode){
            ofSetColor(255);
        } else {
            ofSetColor(ofColor::blueSteel);
        }
        lightningImg.draw(lightningRect);

        ofSetColor(ofColor::blueSteel);

        triangleChangeEnergy *= 0.97;

        ofColor white(255,255,255);
        ofColor blue = ofColor::blueSteel;

        ofColor mix;
        mix = ofColor(triangleChangeEnergy * white.r +
                     (1-triangleChangeEnergy) * blue.r,
                     triangleChangeEnergy * white.g +
                                         (1-triangleChangeEnergy) * blue.g,
                                         triangleChangeEnergy * white.b +
                                                             (1-triangleChangeEnergy) * blue.b);

        ofSetColor(mix);

        ofNoFill();
        //triangleRect

        //triangleRect.set(650-32, 75-32, 920-650, 64);

        ofRectangle brushInnerRect = triangleRect;




        brushImgMix.draw(triangleRect);
//        brushImgInner.getTextureReference().drawSubsection(triangleRect.x, triangleRect.y, triangleRect.width, triangleRect.height,
//        										0,0,1,1);

        //draw(triangleRect);
        ofSetColor(ofColor::blueSteel);

        trashImg.draw(trashRect);

        arrowImg.draw(arrowRect);



     menuFbo->end();

     ofSetColor(255);
    
    ofPushMatrix();
    ofTranslate((-menuFbo->getWidth()*scale + 250*scale) * (1-menuOpenPct),0);
    ofScale(scale,scale, 1);
    menuFbo->draw(0,0);
    ofPopMatrix();
}

void menu::handleMenuPress(ofPoint pt, bool bDrag){

    
    pt *= (1.0 / (float)scale);
    
	ofApp * app= (ofApp*)ofGetAppPtr();


	 ofRectangle camRectBig = camRect;//(50-32, 75-32, 64,64);
	 ofRectangle infoRectBig = infoRect;//(250-32, 75-32, 64,64);
	 ofRectangle lightningRectBig = lightningRect;//(450-32, 75-32, 64,64);
	 ofRectangle triangleRectBig = triangleRect;//(650-32, 75-32, 920-650, 64);
	 ofRectangle arrowRectBig = arrowRect;//(1050-32,75-32, 64, 64);
	 ofRectangle trashRectBig = trashRect;//(1050-32,75-32, 64, 64);



    camRectBig.scaleFromCenter(1.2);
    infoRectBig.scaleFromCenter(1.2);
    lightningRectBig.scaleFromCenter(1.2);
    triangleRectBig.scaleFromCenter(1.2);
    arrowRectBig.scaleFromCenter(1.2);
    trashRectBig.scaleFromCenter(1.2);

    if (!bDrag){
        if (arrowRectBig.inside(pt)){
            bMenuOpen = false;

        }

        if (infoRectBig.inside(pt)){
        	app->IS.setupTitle();
        }

        if (lightningRectBig.inside(pt)){
        	app->bLightningMode = !app->bLightningMode;
        }

        if (camRectBig.inside(pt)){
        	if (app->bRecordingProcessing == false){
        		app->gifEncoder.reset();
        		app->bRecordingMode = true;
        		app->recordingStartTime = ofGetElapsedTimeMillis();
        	}
        }

        if (trashRectBig.inside(pt)){


        	app->line.line.clear();
        	app->lines.clear();

        }
    }

    if (triangleRectBig.inside(pt)){
    	triPct = 1.0 - (pt - triangleRect.getPosition()).x / (float)triangleRect.getWidth();
    	triangleChangeEnergy = 1;
    	mixBrushImage();
    }

}



