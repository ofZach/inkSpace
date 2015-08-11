#include "ofApp.h"
#include "appSettings.h"



#ifdef ANDROID_VERSION
    void loadFileIntoShareSheet(string fileName){
        JNIEnv *env = ofGetJNIEnv();
        jclass localClass = env->FindClass("cc/openframeworks/inkSpace/OFActivity");
        jclass javaClass = (jclass) env->NewGlobalRef(localClass);
        jstring jstr = env->NewStringUTF(fileName.c_str());
        jmethodID method = env->GetStaticMethodID(javaClass, "shareImage", "(Ljava/lang/String;)V");
        env->CallStaticVoidMethod(javaClass, method, jstr);
    }
#endif


void ofApp::onGifSaved(string & fileName){
    ofLog() << "saved gif " << fileName << endl;
    ofFile temp(ofToDataPath(fileName));
    if (temp.exists()){
        gifPath = ofToDataPath(fileName);
        bGifSaved = true;
        
    }
}

//--------------------------------------------------------------
void ofApp::setup(){
    
    
#ifndef ANDROID_VERSION
    ofDisableArbTex();
    screenWidth = ofGetWidth()*2;
    screenHeight = ofGetHeight()*2;
    testFbo.allocate(screenWidth, screenHeight);
    //ofSetFrameRate(30);
#else
    screenWidth = ofGetWidth();
    screenHeight = ofGetHeight();
#endif
    

    bFirstPressOfApp = true;


    IS.screenWidth = screenWidth;
    IS.screenHeight = screenHeight;
    
    
    gifWidth = 540;
	gifHeight = 583;
    
    
	IS.introScreenEnergy = 1.0;
	IS.introScreenEnergyTarget = 1.0;
    
	recordPct = 0;
	bGifSaved = false;
	bRecordingProcessing = false;
	gifPath = "";
	lastGifRecordTime = ofGetElapsedTimeMillis();
    gifRecordRate = (1/20.0) * 1000;
	
    
    ink.loadImage("ink.png");
    grey.loadImage("bg.jpg");
    IS.title.loadImage("title.png");
    
#ifdef ANDROID_VERSION
	ofxAccelerometer.setup();
#endif
    
	ofSetLineWidth(10);
	ofBackground(0,0,0);
    
    
    
    
//    ofxGuiSetBitmapFont();
//
//    ofxGuiSetTextPadding(20);
//    ofxGuiSetDefaultWidth(500);
//    ofxGuiSetDefaultHeight(100);
    
    //gui.setup("panel"); // most of the time you don't need a name but don't forget to call setup
    

    //----------------------------------------------------------------
    rotateAmountInfo.set( "rotateAmountInfo", 9.3,  0.0, 50.0 );
    rotateDampingInfo.set( "rotateDampingInfo", 0.9,  0.0, 1.0 );
    transAmountInfo.set( "transAmountInfo",112.8,  0.0, 200.0  );
    transDampingInfo.set( "transDampingInfo",0.9,  0.0, 1.0 );
    //----------------------------------------------------------------
    
    
    
    /*gui.add(*/accelSmoothing.set( "accelSmoothing", 0.248,  0.0, 1.0 );//);
    /*gui.add(*/rotateAmount.set( "rotateAmount", 28.7,  0.0, 50.0 );//);
    /*gui.add(*/rotateDamping.set( "rotateDamping", 0.9,  0.0, 1.0 );//);
    /*gui.add(*/transAmount.set( "transAmount", 0,  0.0, 200.0 );//);
    /*gui.add(*/transDamping.set( "transDamping",  0.9,  0.0, 1.0  );//);
    /*gui.add(*/minWidth.set( "minWidth",  30,0,200  );//);
    /*gui.add(*/maxWidth.set( "maxWidth",  150, 0, 300  );//);
    //gui.setPosition(-5000,0);
    //----------------------------------------------------------------
    
    
    ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);
    
    
    bIsThisMouseEventInMenu = false;
    bLightningMode = false;
    
    data = new unsigned char[ gifWidth * gifHeight * 3];
    
    gifEncoder.setup(gifWidth, gifHeight, 1/60.0,64);
    //gifEncoder.setDitherMode(OFX_GIF_DITHER_FS);
    
    
   	vbo = new ofVbo();
    shader = new ofShader();
    subscreen = new ofFbo();
    wholeScreen = new ofFbo();
    
    screen = new ofTexture();
    screen->allocate(screenWidth, screenHeight, GL_RGBA);
    subscreen->allocate(gifWidth, gifHeight, GL_RGBA);
    subscreenPixels.allocate(gifWidth, gifHeight, OF_PIXELS_RGBA);
    wholeScreen->allocate(screenWidth, screenHeight);
    
    shader->load("shaders/lineShader");
    
    subscreen->begin();
    ofClear(ofColor::cyan);
    subscreen->end();
    
    bRecordingMode = false;
    
    menuPanel.setup();
    
    // let's adjust scale based on screenWidth vs. 1080p
    
    float pct = ((float)screenWidth) / 1050.0;
    menuPanel.scale = pct;
    

    experimentImg.loadImage("experiment.png");
    recordingImg.loadImage("recording.png");
	processingImg.loadImage("exporting.png");
    closeImg.loadImage("close.png");
    IS.infoScreenDir.listDir("infoScreen");
    
    update();
    update();
    IS.setupTitle();

}



float rotateX = 0;
float rotateY = 0;
float rotateZ = 0;
ofPoint prevAccelNormalized;
float transX = 0;
float transY = 0;

//--------------------------------------------------------------
void ofApp::update(){

    
    menuPanel.update();
    
    
	IS.introScreenEnergy = 0.95 * IS.introScreenEnergy +
    0.05 * IS.introScreenEnergyTarget;
    
	line.update();
	for (int i = 0; i < lines.size(); i++){
		lines[i].update();
	}
    
	if (IS.introScreenEnergy > 0.01){
        
		for (int i = 0; i < IS.linesForIntroScreen.size(); i++){
			IS.linesForIntroScreen[i].update();
		}
	}
    
    
    
	if (bGifSaved){
        bRecordingProcessing = false;
        
        #ifdef ANDROID_VERSION
            loadFileIntoShareSheet(gifPath);
        #endif
        
		bGifSaved = false;
	}
    
    
    float screenScale = ((float)screenWidth) / 1080.0;
	minWidth = ofMap(menuPanel.triPct, 0, 1, 15, 60) * screenScale;
	maxWidth = ofMap(menuPanel.triPct, 0, 1, 50, 250) * screenScale;
   
    #ifdef ANDROID_VERSION
        accel = ofxAccelerometer.getForce();
    #else
    accel = ofPoint(0,0,0);
    #endif

	normAccel = (1-accelSmoothing) * normAccel + (accelSmoothing) * accel.getNormalized();

    if (ofGetKeyPressed('a')) rotateX = 1;
    if (ofGetKeyPressed('s')) rotateX = -1;
    if (ofGetKeyPressed('z')) rotateY = 1;
    if (ofGetKeyPressed('x')) rotateY = -1;
    if (ofGetKeyPressed('q')) rotateZ = 1;
    if (ofGetKeyPressed('w')) rotateZ = -1;
    
    ofPoint diff = normAccel - prevAccelNormalized;

    prevAccelNormalized =  normAccel;
    
    //normalizedSmoothed = (1-accelSmooth) * normalizedSmoothed + accelSmooth * diff * 10.0;
    
    
    float rotateAmountMix = IS.introScreenEnergy * rotateAmountInfo +
    (1-IS.introScreenEnergy) * rotateAmount;
    float rotateDampingMix = IS.introScreenEnergy * rotateDampingInfo +
    (1-IS.introScreenEnergy) * rotateDamping;
    float transAmountMix = IS.introScreenEnergy * transAmountInfo +
    (1-IS.introScreenEnergy) * transAmount;
    float transDampingMix = IS.introScreenEnergy * transDampingInfo +
    (1-IS.introScreenEnergy) * transDamping;
    
    
    float startupScale = ofMap(ofGetFrameNum(), 0, 45, 0, 1, true);


    rotateX -= diff.y * rotateAmountMix * startupScale;
    rotateY -= diff.x * rotateAmountMix * startupScale;
    rotateX *= rotateDampingMix;
    rotateY *= rotateDampingMix;
    transX -= diff.x * transAmountMix * startupScale;
    transY += diff.y * transAmountMix * startupScale;
    transX *= transDampingMix;
    transY *= transDampingMix;
    
    ofMatrix4x4 rotateMatrixa;
    ofMatrix4x4 rotateMatrixb;
    ofMatrix4x4 rotateMatrixc;
    rotateMatrixa.makeRotationMatrix( rotateX, 1,0,0);
    rotateMatrixb.makeRotationMatrix( rotateY, 0, 1, 0);
    rotateMatrixc =  rotateMatrixa * rotateMatrixb;
    rotateMatrixc.translate(transX, transY, 0);
    
    
    
    ofMatrix4x4 flip;
    flip.makeScaleMatrix(1,-1,1);
    
    for (int i = 0; i < IS.titlePts.size(); i++){
        ofPoint fromCenter = IS.titlePts[i] - ofPoint(screenWidth/2, screenHeight/2);
        ofPoint rot = fromCenter  * rotateMatrixc + ofPoint(screenWidth/2, screenHeight/2);
        IS.titlePts[i] = rot;
    }
    
    if (IS.introScreenEnergy < 0.99){
	    for (int i = 0; i < line.line.getVertices().size(); i++){
	        ofPoint fromCenter = line.line.getVertices()[i] - ofPoint(screenWidth/2, screenHeight/2);
	        ofPoint rot = fromCenter * rotateMatrixc + ofPoint(screenWidth/2, screenHeight/2);
	        line.line.getVertices()[i] = rot;
            
	    }
        
        
	    for (int i = 0; i < lines.size(); i++){
	        for (int j = 0; j < lines[i].line.getVertices().size(); j++){
	            ofPoint fromCenter = lines[i].line.getVertices()[j] - ofPoint(screenWidth/2, screenHeight/2);
	            ofPoint rot = fromCenter * rotateMatrixc + ofPoint(screenWidth/2, screenHeight/2);
	            lines[i].line.getVertices()[j] = rot;
	        }
	    }
    }
    
    if (IS.introScreenEnergy > 0.01){
	    for (int i = 0; i < IS.linesForIntroScreen.size(); i++){
            for (int j = 0; j < IS.linesForIntroScreen[i].line.getVertices().size(); j++){
                ofPoint fromCenter = IS.linesForIntroScreen[i].line.getVertices()[j] - ofPoint(screenWidth/2, screenHeight/2);
                ofPoint rot = fromCenter * rotateMatrixc + ofPoint(screenWidth/2, screenHeight/2);
                IS.linesForIntroScreen[i].line.getVertices()[j] = rot;
            }
	    }
    }
    
    
    

}

void ofApp::exportDrawingsToFile(){
    
	ofLog() << "starting save" << endl;
	ofxXmlSettings xml;
	xml.setVerbose(true);
	xml.addTag("drawing");
	xml.pushTag("drawing");
	for (int i = 0; i < lines.size(); i++){
		xml.addTag("line");
		xml.pushTag("line", i);
		lines[i].writeToXmlFile(xml);
		xml.popTag();
	}
	xml.popTag();
    
	string fileName = "output/" + ofGetTimestampString() + ".xml";
	xml.saveFile(fileName);
	ofLog() << fileName << endl;
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    #ifndef ANDROID_VERSION
        testFbo.begin();
    #endif

    
    if (ofGetFrameNum() == 0){	// for some reason, the vbo is weird on the first frame
		delete vbo;
		vbo = new ofVbo();
	}
    
    
	if (vbo == NULL) return;
    
    
	wholeScreen->begin();
    
	if (!bRecordingMode){
		ofSetColor(180);
		grey.draw(0,0,wholeScreen->getWidth(), wholeScreen->getHeight());
	} else {
		ofSetColor(0);
		ofFill();
		ofRect(0,0,wholeScreen->getWidth(), wholeScreen->getHeight());
	}
	ofSetColor(255);
    
	if ( IS.introScreenEnergy > 0.01){
		ofSetColor(255,255,255,IS.introScreenEnergy*255);
		IS.drawTitle();
	}
    //	ofSetRectMode(OF_RECTMODE_CENTER);
    //	title.draw(screenWidth/2, screenHeight/2, title.width, -title.height);
    //	ofSetRectMode(OF_RECTMODE_CORNER);
    
    //ofBackgroundGradient(ofColor(30,30,30), ofColor(0,0,0));
    
    if (bRecordingMode == true){
        
        
    	if ((ofGetElapsedTimeMillis() - recordingStartTime) > 10000){
            
    		ofDirectory output("output");
    		if (!output.exists()){
    			output.create();
    		}
    		string fileName = "output/" + ofToString((int)ofRandom(0,10000)) + ".gif";
    		ofFile blah(ofToDataPath(fileName));
    		if (!blah.exists()){
    			blah.create();
    		}
            gifEncoder.setFrameDuration( 1.0 / 20.0);
    	    gifEncoder.save(  fileName );
    		bRecordingMode = false;
    		bRecordingProcessing = true;
            
    	} else {
            
    		recordPct = (ofGetElapsedTimeMillis() - recordingStartTime) / 10000.0;
            
            
    	}
    }
    
    
    //---------------------------------------------------- why why why why
    ofPushMatrix();
    ofTranslate(screenWidth/2, screenHeight/2);
    
    ofSetColor(0,0,0,0);
    //ofSetColor(255,255,0,0);
    ofLine(0,0,normAccel.x*screenWidth/2,0);
    //ofSetColor(255,0,255, 0);
    ofLine(0,0,0,-normAccel.y*screenHeight/2);
    
    // we don't draw z as the perspective might be confusing
    // but it's approximately one when the device is still and parallel
    // to the ground
    ofPopMatrix();
    
    ofCamera cam;
    cam.setupPerspective();
    
    
    ofSetColor(255);
    
    
    if(bRecordingMode == true){
        
    	cam.setPosition(cam.getPosition() + ofPoint(0,0,300));
    }
    
    vector < ofVec3f > pts;
    //vector < ofVec2f > texCoords;
    
    
    ofMatrix4x4 worldFlipMatrix;
    //temp.makeIdentityMatrix();
    worldFlipMatrix.makeScaleMatrix(1,-1,1);
    //-------------- setup shader
    
    
    
    
    shader->begin();
    shader->setUniform1i("miter", 0);
    //shader.setUniform1f("thickness", 30);
    shader->setUniform1f("aspect", screenWidth/(float)screenHeight);
    shader->setUniformMatrix4f("projection",cam.getProjectionMatrix());
    shader->setUniformMatrix4f("modelview", cam.getModelViewMatrix() * worldFlipMatrix);
    shader->setUniformTexture("ink", ink, 0);
    shader->setUniform1f("focusPoint", -1400);
    shader->setUniform1f("drawTrans", 0.0); //(int) ofGetElapsedTimef() % 6 > 3 ? 0.0 : 1.0);
    //shader->setUniform1f("time", ofGetElapsedTimef());	// fuck
    
    
    
    
    
    vector < ofPoint > currentPoints;
    vector < ofPoint > prevPositions;
    vector < ofPoint > nextPositions;
    vector < ofPoint > lineInfo;                 // could this be in one vec3?
    
    bool bDrawingCurrentLine = line.line.size() > 1 ? true : false;
    
    if (IS.introScreenEnergy < 0.99){
		for (int i = 0; i < lines.size(); i++){
			addPolyline(lines[i],
                        currentPoints,
						prevPositions,
						nextPositions,
						lineInfo,
						i == 0 ? false : true,
						(i == lines.size()-1 && !bDrawingCurrentLine) ? false : true,
                        (1-IS.introScreenEnergy));
		}
        
		if (line.line.size() > 1){
			addPolyline(line,
						currentPoints,
						prevPositions,
						nextPositions,
						lineInfo,
						true,
						true,
						1.0);
		}
    }
    
    if (IS.introScreenEnergy > 0.01){
    	for (int i = 0; i < IS.linesForIntroScreen.size(); i++){
            addPolyline(IS.linesForIntroScreen[i],
                        currentPoints,
                        prevPositions,
                        nextPositions,
                        lineInfo,
                        i == 0 ? false : true,
                        (i == IS.linesForIntroScreen.size()-1 && true) ? false : true,
                        (IS.introScreenEnergy));
        }
        
        
    }
    
    
    
    
    
    //ofLogError() << "what 8?? ";
    float* ptr = currentPoints[0].getPtr();
    
    //ofLogError() << "what 8.1?? ";
    //vbo->bind();
    
    //ofLogError() << "what 8.2?? ";
    vbo->setAttributeData(shader->getAttributeLocation("position"),
                          ptr,  3, currentPoints.size(), GL_DYNAMIC_DRAW, sizeof(ofPoint));
    vbo->setAttributeData(shader->getAttributeLocation("previous"),
                          prevPositions[0].getPtr(),  3, prevPositions.size(), GL_DYNAMIC_DRAW, sizeof(ofPoint));
    vbo->setAttributeData(shader->getAttributeLocation("next"),
                          nextPositions[0].getPtr(),  3, nextPositions.size(), GL_DYNAMIC_DRAW, sizeof(ofPoint));
    
    vbo->setAttributeData(shader->getAttributeLocation("lineInfo"),
                          lineInfo[0].getPtr(),  3, lineInfo.size(), GL_DYNAMIC_DRAW, sizeof(ofPoint));
    
    vbo->setVertexData(&currentPoints[0], currentPoints.size(), GL_DYNAMIC_DRAW);
    //vbo.setTexCoordData(&texCoords[0], texCoords.size(), GL_DYNAMIC_DRAW);
    
    
    //ofDisableDepthTest();
    //ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    //ink.bind();
    //ofLogError() << "what 8.4?? ";
    
    ofSetColor(255);
    
    
    vbo->draw(GL_TRIANGLE_STRIP, 0, currentPoints.size());
    
    
    //ofLogError() << "what 8.5?? ";
    
    //ofLogError() << "what 8.6?? ";
    //vbo->unbind();
    shader->end();
    
    //ofLogError() << "what 8.7?? ";
    ofEnableAlphaBlending();
    
    
    //ofLogError() << "what 9?? ";
    
    
    wholeScreen->end();
    //return;
    
    
    //06-12 21:14:23.753: I/(7795): /storage/emulated/0/Android/data/cc.openframeworks.accelDraw/files/
    
    
    // screen->loadScreenData(0,0, screenWidth, screenHeight);
    
    if (bRecordingMode == true &&ofGetFrameNum() % 2 == 0){
        
    	lastGifRecordTime = ofGetElapsedTimeMillis();
        
        
    	subscreen->begin();
    	ofSetColor(255);
        
		// TODO THIS
        
		ofRectangle offscreen(0,0, screenWidth, screenHeight);
		ofRectangle gifRect(0,0, gifWidth, gifHeight);
		offscreen.scaleTo(gifRect, OF_SCALEMODE_FILL);
        
		wholeScreen->draw(offscreen.x, offscreen.y + offscreen.height, offscreen.width, -offscreen.height);
    	subscreen->end();
        
		if (true){
			subscreen->readToPixels(subscreenPixels);
            
			unsigned char * temp  = subscreenPixels.getPixels();
			//for (int j = 0; j < 3; j++){
            
			for (int i = 0; i < subscreenPixels.getWidth() * subscreenPixels.getHeight(); i++){
				data[i*3] = temp[i * 4+0];
				data[i*3+1] = temp[i * 4+0];
				data[i*3+2] = temp[i * 4+0];
			}
			gifEncoder.addFrame(
                                data,
                                subscreenPixels.getWidth(),
                                subscreenPixels.getHeight(),
                                24,
                                1.0/60.0f
                                );
		}
        
		//}
    }
    
    ofSetColor(255);
    wholeScreen->draw(0,screenHeight, screenWidth, -screenHeight);
    
    //gui.draw();
    
    if (!bRecordingMode == true){
    	menuPanel.draw();
    }
    
    if (bRecordingMode == true){
        
        
    	ofFill();
        
    	ofColor tempColor = ofColor::steelBlue;
        
    	ofSetColor(tempColor.r, tempColor.g, tempColor.b, ofMap(sin(ofGetElapsedTimef() * 2.5), -1, 1, 180, 220));
    	ofRectangle tempRect(0,0,screenWidth, 150);
    	tempRect.x += 50;
    	tempRect.y += 50;
    	tempRect.width -= 100;
    	tempRect.height -= 100;
        
    	//tempRect.scaleFromCenter(0.75);
    	ofRect(tempRect.x, tempRect.y,tempRect.width * recordPct, tempRect.height);
        
        
        
    	ofSetLineWidth(1);
        
        
    }
    
    
    
    float screenScale = ((float)screenWidth) / 1080.0;
    
    if(bRecordingMode == true){
        
    	ofColor tempColor = ofColor::steelBlue;
    	ofSetColor(tempColor.r, tempColor.g, tempColor.b, 80);
    	ofRectangle gifRect(0,0,gifWidth, gifHeight);
    	gifRect.scaleTo(ofRectangle(0,0, screenWidth, screenHeight));
    	ofFill();
    	ofRect(gifRect);
        
        ofSetColor(255);
        
        ofPushMatrix();
        ofTranslate(screenWidth/2, screenHeight-100);
        ofScale(screenScale, screenScale);
        ofSetRectMode(OF_RECTMODE_CENTER);
        recordingImg.draw(0,0);
        ofSetRectMode(OF_RECTMODE_CORNER);
        ofPopMatrix();
        
        ofRectangle tempRect(screenWidth/2 - recordingImg.width/2, screenHeight-100 - recordingImg.height/2, recordingImg.width, recordingImg.height);
        tempRect.scaleFromCenter(screenScale);
        
        closeImageRect.set(tempRect.x + tempRect.width + 35*screenScale, tempRect.y + tempRect.height*0.2, tempRect.height * 0.6, tempRect.height*0.6);

        ofSetColor(ofColor::steelBlue);
        closeImg.draw( closeImageRect );
        
        closeImageRect.scaleFromCenter(3.0);

        
    }
    //ofLog() << "bRecordingProcessing ? " << bRecordingProcessing << endl;
    
    if (bRecordingProcessing == true ){
        
    	ofFill();
    	ofSetColor(255, 255, 255, 255 * (1-IS.introScreenEnergy));
        
        ofPushMatrix();
        ofTranslate(screenWidth/2, screenHeight-100);
        ofScale(screenScale, screenScale);
        ofSetRectMode(OF_RECTMODE_CENTER);
        processingImg.draw(0,0);
        ofSetRectMode(OF_RECTMODE_CORNER);
        ofPopMatrix();
    }
    /*
     *
     *  if (recording){
     *     cacl recording pct
     *     draw something useful
     *  }
     *  else if (compresison){
     *    calc compression pct
     *  }
     *
     *
     *
     *
     *
     */
    
    // gui.draw();
    
    if (IS.introScreenEnergy > 0.01){
        
    	ofSetColor(255,255,255, IS.introScreenEnergy*255);
        ofSetRectMode(OF_RECTMODE_CENTER);
        experimentImg.draw(screenWidth/2, screenHeight-100);
        ofSetRectMode(OF_RECTMODE_CORNER);
        ofSetColor(255);
    }
    
    
    ofSetLineWidth(1);
    ofTranslate(-screenWidth/2, -screenHeight/2);
    ofSetColor(255,0,0);
    ofDrawBitmapString(ofToString(ofGetFrameRate()),20,20);
    ofSetColor(255);
    
    

#ifndef ANDROID_VERSION
    testFbo.end();
    testFbo.draw(0,0,ofGetWidth(), ofGetHeight());
#endif
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    #ifndef ANDROID_VERSION
        touchMoved(x*2, y*2, 0);
    #endif

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    #ifndef ANDROID_VERSION
        touchDown(x*2, y*2, 0);
    #endif
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    #ifndef ANDROID_VERSION
        touchUp(x*2, y*2, 0);
    #endif
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


//--------------------------------------------------------------
void ofApp::touchDown(int x, int y, int id){
    
	if (bFirstPressOfApp == true){
		menuPanel.bMenuOpen = true;
		menuPanel.lastMenuTouchTime = ofGetElapsedTimeMillis();
		bFirstPressOfApp = false;
	}

	
	if (bRecordingMode){
		if (closeImageRect.inside(x,y)){
			bRecordingMode = false;
			gifEncoder.reset();
			return;
		}

	}
    
    
	if (id == 0){
		if (IS.introScreenEnergyTarget > 0.9){
			IS.introScreenEnergyTarget = 0.0;
			bIsThisMouseEventInMenu = true;
			return;
		}
	}
    
    
	    
    
	if (id == 0){
        
		bIsThisMouseEventInMenu = false;
        
		if (menuPanel.bMenuOpen == false && menuPanel.menuOpenPct < 0.01){
			ofPoint mousePt(x,y);
            
			ofRectangle rect = menuPanel.getBounds();
			rect.scaleFromCenter(1.1);
			if (rect.inside(mousePt)){
				bIsThisMouseEventInMenu = true;
				menuPanel.lastMenuTouchTime = ofGetElapsedTimeMillis();
				menuPanel.bMenuOpen = true;
			}
		}
        
		if (menuPanel.bMenuOpen == true && menuPanel.menuOpenPct > 0.95){
            ofPoint mousePt(x,y);
            ofRectangle rect = menuPanel.getBounds();
            ofRectangle biggerRect = rect;
            biggerRect.scaleFromCenter(1.1);	// increase by 10% :)
            if (biggerRect.inside(mousePt)){
                bIsThisMouseEventInMenu = true;
                menuPanel.handleMenuPress(mousePt - rect.getPosition(), false );
            }
		}
        
        
		if (!bIsThisMouseEventInMenu){
			IS.introScreenEnergyTarget = 0.0;
			line.startDrawing();
			line.bAmDoneDrawing = false;
			line.line.addVertex(ofPoint(x,y));
			line.addedVertex();
		}
	}
}

//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){
    
    
	
    
    
	if (id == 0){
        
        if (bIsThisMouseEventInMenu == false){
            if (line.line.size() > 0){
                ofPoint me(x,y);
                if ((me - line.line[line.line.size()-1]).length() > 3){
                    line.line.addVertex(x,y);
                    line.addedVertex();
                    
                    if (line.line.size() % 10 == 0 && line.line.size() > 20){
                        
                        int posA = (int)((line.line.size()-1) / 10) * 10;
                        int posB = (int)((line.line.size() / 10)) * 10;
                        ofPolyline b,c, d;
                        splitPolylineAt(line.line, b, c, d, posA, posB);
                        //d = d.getSmoothed(3);
                        d = d.getResampledBySpacing(2);
                        //c.simplify(0.5);
                        
                        joinPolyline(line.line, b,c,d); //, <#ofPolyline &c#>)
                    }
                    
                    // smooth near the front
                    
                    int sizeOfBlur = 10;
                    if (line.line.size() > sizeOfBlur){
                        
                        int start = line.line.size()-sizeOfBlur;
                        int end = line.line.size();
                        
                        vector < ofPoint > pts;
                        
                        for (int i = start; i < end; i++){
                            float pct = (i-start) / (float)(sizeOfBlur - 1);
                            float amountOfBlur = sin(pct * PI);
                            
                            int i_m_1 = MAX(0, i-1);
                            int i_p_1 = MIN(line.line.size()-1, i+1);  // todo: replace size
                            
                            float pctMe = ofMap(amountOfBlur, 0, 1, 1.0, 0.6);
                            float pctThem = (1.0 - pctMe) / 2.0;
                            
                            ofPoint pt =  (pctThem * line.line[i_m_1] +
                                           pctMe * line.line[i] + pctThem * line.line[i_p_1]);
                            
                            pts.push_back(pt);
                            
                        }
                        
                        for (int i = start; i < end; i++){
                            //cout << (i-start) << " ::::: " << line[i] << " ------ " << pts[i - start] << endl;
                            //line[i] = 0.8f * line[i]  + 0.2 * pts[i - start];
                            
                            line.line[i] = pts[i-start];
                        }
                        
                        
                    }
                    
                    
                    //line.line = line.line.getResampledBySpacing(3);
                    //  line.line = line.line.getSmoothed(3);
                    
                }
            } else {
                line.line.addVertex(ofPoint(x,y));
                line.addedVertex();
            }
            line.computeWidths();
        } else {
            
            if (menuPanel.bMenuOpen == true && menuPanel.menuOpenPct > 0.95){
                ofPoint mousePt(x,y);
                ofRectangle rect = menuPanel.getBounds();
                ofRectangle biggerRect = rect;
                biggerRect.scaleFromCenter(1.1);
                //((-menu->getWidth() + 250) * (1-menuOpenPct),0, menu->getWidth(), menu->getHeight());
                if (biggerRect.inside(mousePt)){
                    bIsThisMouseEventInMenu = true;
                    
                    menuPanel.lastMenuTouchTime = ofGetElapsedTimeMillis();
                    menuPanel.handleMenuPress(mousePt - rect.getPosition(), true );
                }
            }
            
        }
        
        //line = line.getResampledBySpacing(3.0);
    }
}

//--------------------------------------------------------------
void ofApp::touchUp(int x, int y, int id){
	if (id == 0){
		if (bIsThisMouseEventInMenu == false){
            if (line.line.size() > 1){
                
                // skip ? ?f
                //line.line = line.line.getSmoothed(3);
                
                line.computeWidths();
                line.finishDrawing();
                line.bAmDoneDrawing = true;    
                lines.push_back(line);
                line.bAmDoneDrawing = false;
            }
		    line.line.clear();
        }
    }
}


//--------------------------------------------------------------
void ofApp::touchDoubleTap(int x, int y, int id){
    
    line.line.clear();
    lines.clear();
	
}



//--------------------------------------------------------------
void ofApp::pause(){
	ofLogError() << "pause " << endl;
}


void ofApp::unloadTextures(){
	if (vbo != NULL) delete vbo;
    if (shader != NULL) delete shader;
    if (subscreen != NULL) delete subscreen;
    if (wholeScreen != NULL) delete wholeScreen;
    if (menuPanel.menuFbo != NULL) delete menuPanel.menuFbo;
    if (screen != NULL) delete screen;
    
    vbo = NULL;
}

//--------------------------------------------------------------
void ofApp::stop(){
	ofLogError() << "stop " << endl;
    
}

//--------------------------------------------------------------
void ofApp::resume(){
	ofLogError() << "resume " << endl;
    
}

//--------------------------------------------------------------
void ofApp::reloadTextures(){
	ofLogError("this") << "reload " << endl;
    
    vbo = new ofVbo();
    shader = new ofShader();
    subscreen = new ofFbo();
    wholeScreen = new ofFbo();
    menuPanel.menuFbo = new ofFbo();
    screen = new ofTexture();
    
    //ofLog() << "hi3" << endl;
    menuPanel.menuFbo->allocate(menuPanel.fboRect.width, menuPanel.fboRect.height);
    screen->allocate(screenWidth, screenHeight, GL_RGBA);
    subscreen->allocate(gifWidth, gifHeight, GL_RGBA);
    wholeScreen->allocate(screenWidth, screenHeight);
    shader->load("shaders/lineShader");
	
    
}

//--------------------------------------------------------------
bool ofApp::backPressed(){
	return false;
}

//--------------------------------------------------------------
void ofApp::okPressed(){
    
    
    
}

//--------------------------------------------------------------
void ofApp::cancelPressed(){
    
}



