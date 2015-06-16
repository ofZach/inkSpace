#include "ofApp.h"



void loadFileIntoShareSheet(string fileName){
	JNIEnv *env = ofGetJNIEnv();
	jclass localClass = env->FindClass("cc/openframeworks/accelDraw/OFActivity");
	ofLog() << localClass;

	jclass javaClass = (jclass) env->NewGlobalRef(localClass);

	ofLog() << javaClass;

	jstring jstr = env->NewStringUTF(fileName.c_str());

	ofLog() << jstr;


	jmethodID method = env->GetStaticMethodID(javaClass, "sayHi", "(Ljava/lang/String;)V");

ofLog() << method;

	env->CallStaticVoidMethod(javaClass, method, jstr);
}

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


	bGifSaved = false;
	gifPath = "";

	ofLogNotice() << ofToDataPath("");

    camImg.loadImage("cam.png");
    infoImg.loadImage("info.png");
    lightningImg.loadImage("lightning.png");
    arrowImg.loadImage("menu.png");

    //06-12 20:25:43.772: I/(6319): /storage/emulated/0/Android/data/cc.openframeworks.accelDraw/files/


    ofLog() << camImg.width << " ??? " << endl;

	// initialize the accelerometer
	ofxAccelerometer.setup();

	font.loadFont("verdana.ttf",24);
	ofSetLineWidth(10);
	ofBackground(0,0,0);


	nPointsLastFrame = 0;

    lastNumPts = 0;

    //ink.loadImage("test4.png");

    vbo =  NULL; //new ofVbo();

    bNeedToReallocateOpengl = true;
    nFramesToReallocate = 0;

    //------------------------------ gui
    ofxGuiSetFont("verdana.ttf",18,true,true);
    ofxGuiSetTextPadding(20);
    ofxGuiSetDefaultWidth(500);
    ofxGuiSetDefaultHeight(100);

    gui.setup("panel"); // most of the time you don't need a name but don't forget to call setup

    //gui.add(accelSmooth.set( "accelSmooth", 0.04,  0.0, 1.0 ));
    gui.add(rotateAmount.set( "rotateAmount", 9.3,  0.0, 50.0 ));
    gui.add(rotateDamping.set( "rotateDamping", 0.9,  0.0, 1.0 ));
    gui.add(transAmount.set( "transAmount", 112.8,  0.0, 200.0 ));
    gui.add(transDamping.set( "transDamping",  0.9,  0.0, 1.0  ));
    gui.add(minWidth.set( "minWidth",  30,0,200  ));
    gui.add(maxWidth.set( "maxWidth",  150, 0, 300  ));
    
    
    ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);


//    gui.add(filled.set("bFill", true));
//    gui.add(radius.set( "radius", 140, 10, 300 ));
//    gui.add(center.set("center",ofVec2f(ofGetWidth()*.5,ofGetHeight()*.5),ofVec2f(0,0),ofVec2f(ofGetWidth(),ofGetHeight())));
//    gui.add(color.set("color",ofColor(100,100,140),ofColor(0,0),ofColor(255,255)));
//    gui.add(circleResolution.set("circleRes", 5, 3, 90));
//    gui.add(twoCircles.setup("twoCircles"));
//    gui.add(ringButton.setup("ring"));
//    gui.add(screenSize.set("screenSize", ofToString(ofGetWidth()) + "x" + ofToString(ofGetHeight())));

    	//gui.unregisterMouseEvents();

    	//gui.setVisible(false);

    //ofRegisterTouchEvents((ofxAndroidApp*)this);

    menu.allocate(1200,150);

    bMenuOpen = false;
    menuOpenPct = 0;
    bIsThisMouseEventInMenu = false;
    bLightningMode = false;
    triPct = 0.5;



    screen.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    subscreen.allocate(ofGetWidth()/4, ofGetHeight()/4);
    subscreenPixels.allocate(ofGetWidth()/4, ofGetHeight()/4, OF_PIXELS_RGBA);

    wholeScreen.allocate(ofGetWidth(), ofGetHeight());

    data = new unsigned char[ (ofGetWidth()/4) * (ofGetHeight()/4) * 3];
    ofLog() << "subscreenPixels" << subscreenPixels.getWidth() << " " << subscreenPixels.getHeight();
    ofLog() << "subscreen" << subscreen.getWidth() << " " << subscreen.getHeight();


    subscreen.begin();
    ofClear(ofColor::cyan);
    subscreen.end();

    gifEncoder.setup(ofGetWidth()/4, ofGetHeight()/4, 1/30.0, 256);


    //loadFileIntoShareSheet("/storage/emulated/0/Android/data/cc.openframeworks.accelDraw/files/output/1974.gif");
    //trySomething();
}



float rotateX = 0;
float rotateY = 0;
float rotateZ = 0;
ofPoint prevAccelNormalized;
float transX = 0;
float transY = 0;

//--------------------------------------------------------------
void ofApp::update(){


	if (bGifSaved){
		loadFileIntoShareSheet(gifPath);
		bGifSaved = false;
	}

	minWidth = ofMap(triPct, 0, 1, 15, 60);
	maxWidth = ofMap(triPct, 0, 1, 50, 250);


	if (bMenuOpen){
		menuOpenPct = 0.9f * menuOpenPct + 0.1f * 1.0;
	} else {
		menuOpenPct = 0.9f * menuOpenPct + 0.1f * 0.0;
	}


	menu.begin();
	    ofClear(0,0,0,180 * menuOpenPct);
	    ofSetColor(ofColor::blueSteel);

	    ofSetRectMode(OF_RECTMODE_CENTER);
	    camImg.draw(50, 75);
	   	infoImg.draw(250, 75);

	   	if (bLightningMode){
	   		ofSetColor(255);
	   	} else {
	   		ofSetColor(ofColor::blueSteel);
	   	}
	   	lightningImg.draw(450, 75);

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
	   	ofPoint a(650-32, 75+32);
	   		   	ofPoint b(650-32, 75-32);
	   		   	ofPoint c(920, 75);
	   	ofTriangle(a,b,c);
	   	ofFill();
	   	ofTriangle( triPct * a + (1-triPct) * c, triPct * b + (1-triPct) * c, c);
	   	ofSetColor(ofColor::blueSteel);


	   	if (!bMenuOpen){
	   		arrowImg.draw(1050,75);
	   	} else {
	   		arrowImg.draw(1050,75, -64, 64);
	   	}
	   	ofSetRectMode(OF_RECTMODE_CORNER);
	menu.end();


	accel = ofxAccelerometer.getForce();
	messages[0] = "g(x) = " + ofToString(accel.x,2);
	messages[1] = "g(y) = " + ofToString(accel.y,2);
	messages[2] = "g(z) = " + ofToString(accel.z,2);
	normAccel = accel.getNormalized();

	    if (ofGetKeyPressed('a')) rotateX = 1;
	    if (ofGetKeyPressed('s')) rotateX = -1;
	    if (ofGetKeyPressed('z')) rotateY = 1;
	    if (ofGetKeyPressed('x')) rotateY = -1;
	    if (ofGetKeyPressed('q')) rotateZ = 1;
	    if (ofGetKeyPressed('w')) rotateZ = -1;


	    ofPoint diff = normAccel - prevAccelNormalized;

	    prevAccelNormalized =  normAccel;

	    //normalizedSmoothed = (1-accelSmooth) * normalizedSmoothed + accelSmooth * diff * 10.0;


	    rotateX -= diff.y * rotateAmount;
	    rotateY -= diff.x * rotateAmount;
	    rotateX *= rotateDamping;
	    rotateY *= rotateDamping;
	    transX -= diff.x * transAmount;
	    transY += diff.y * transAmount;
	    transX *= transDamping;
	    transY *= transDamping;

        ofMatrix4x4 rotateMatrixa;
		ofMatrix4x4 rotateMatrixb;
		ofMatrix4x4 rotateMatrixc;
		rotateMatrixa.makeRotationMatrix( rotateX, 1,0,0);
		rotateMatrixb.makeRotationMatrix( rotateY, 0, 1, 0);
		rotateMatrixc =  rotateMatrixa * rotateMatrixb;
		rotateMatrixc.translate(transX, transY, 0);

	    for (int i = 0; i < line.line.getVertices().size(); i++){
	        ofPoint fromCenter = line.line.getVertices()[i] - ofPoint(ofGetWidth()/2, ofGetHeight()/2);
	        ofPoint rot = fromCenter * rotateMatrixc + ofPoint(ofGetWidth()/2, ofGetHeight()/2);
	        line.line.getVertices()[i] = rot;
	    }


	    for (int i = 0; i < lines.size(); i++){
	        for (int j = 0; j < lines[i].line.getVertices().size(); j++){
	            ofPoint fromCenter = lines[i].line.getVertices()[j] - ofPoint(ofGetWidth()/2, ofGetHeight()/2);
	            ofPoint rot = fromCenter * rotateMatrixc + ofPoint(ofGetWidth()/2, ofGetHeight()/2);
	            lines[i].line.getVertices()[j] = rot;
	        }
	    }
}

void checkError( string where){
	GLenum err;
	ofLog() << "checking " << where << endl;
	while ((err = glGetError()) != GL_NO_ERROR) {
	      ofLog() << where << " - " << "OpenGL error: " << err << endl;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){




	if (bNeedToReallocateOpengl == true){
	if (vbo != NULL){
		delete vbo;
	}
	// this seem rediculous

	vbo = new ofVbo();
	//vbo.clear();
	ink.loadImage("test4.png");
	shader.load("test");

	menu.allocate(1200,150);
	screen.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	subscreen.allocate(ofGetWidth()/4, ofGetHeight()/4);
	//subscreenPixels.allocate(ofGetWidth()/4, ofGetHeight()/4, OF_PIXELS_RGBA);
	wholeScreen.allocate(ofGetWidth(), ofGetHeight());


	if (nFramesToReallocate > 10){
		bNeedToReallocateOpengl = false;
	}
	nFramesToReallocate++;
	}


	//checkError("a");


    //ofBackground(80);

	wholeScreen.begin();

    ofBackgroundGradient(ofColor(30,30,30), ofColor(0,0,0));

    if (bRecordingMode == true){
    	ofSetColor(255,0,0);
    	ofCircle(0,0,10);
    	ofSetColor(255);

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
    	    gifEncoder.save(  fileName );
    		bRecordingMode = false;

    	}
    }
//    ofSetColor(255,255,0);
//    font.drawString(messages[0],10,font.stringHeight(messages[0])+20);
//    ofSetColor(255,0,255);
//    font.drawString(messages[1],10,(font.stringHeight(messages[0])+20)*2);
//    ofSetColor(0,255,255);
//    font.drawString(messages[2],10,(font.stringHeight(messages[0])+20)*3);
//    ofSetColor(255,255,255);
//    font.drawString(ofToString(ofGetFrameRate()),10,(font.stringHeight(messages[0])+20)*4);


//    ofFill();
//    ofSetColor(80,80,80, 200);
//    //ofRect(0, ofGetHeight()-150, 800, 150);
//    ofSetColor(255,255,255,100);
//    ofRect(0, ofGetHeight()-150, 500, 150);
//    camImg.draw(50, ofGetHeight() - 150);
//    infoImg.draw(250, ofGetHeight() - 150);
//    lightningImg.draw(450, ofGetHeight() - 150);


    //gui.draw();

    //---------------------------------------------------- why why why why
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);

    ofSetColor(0,0,0,0);
    //ofSetColor(255,255,0,0);
    ofLine(0,0,normAccel.x*ofGetWidth()/2,0);
    //ofSetColor(255,0,255, 0);
    ofLine(0,0,0,-normAccel.y*ofGetHeight()/2);

    // we don't draw z as the perspective might be confusing
    // but it's approximately one when the device is still and parallel
    // to the ground
    ofPopMatrix();

    ofCamera cam;
    cam.setupPerspective();


    //checkError("b");


    ofSetColor(255);
    //	line.draw();
    //	    for (int i = 0; i < lines.size(); i++){
    //	        lines[i].draw();
    //	    }





    vector < ofVec3f > pts;
    //vector < ofVec2f > texCoords;


    ofMatrix4x4 worldFlipMatrix;
    //temp.makeIdentityMatrix();
    worldFlipMatrix.makeScaleMatrix(1,-1,1);
    //-------------- setup shader


    //checkError("c");

    shader.begin();
    shader.setUniform1i("miter", 0);
    //shader.setUniform1f("thickness", 30);
    shader.setUniform1f("aspect", ofGetWidth()/(float)ofGetHeight());
    shader.setUniformMatrix4f("projection",cam.getProjectionMatrix());
    shader.setUniformMatrix4f("modelview", cam.getModelViewMatrix() * worldFlipMatrix);
    shader.setUniformTexture("ink", ink, 0);
    shader.setUniform1f("focusPoint", -1400);
    //shader.setUniform1f("time", ofGetElapsedTimef());	// fuck



    //checkError("d");

    //ofLog() << -1500 + 500 * sin(ofGetElapsedTimef()/3.0) << endl;

    vector < ofPoint > currentPoints;
    vector < ofPoint > prevPositions;
    vector < ofPoint > nextPositions;
    vector < ofPoint > lineInfo;                 // could this be in one vec3?

    for (int i = 0; i < lines.size(); i++){
        addPolyline(lines[i],
                currentPoints,
                    prevPositions,
                    nextPositions,
                    lineInfo);
    }

    addPolyline(line,
                currentPoints,
                prevPositions,
                nextPositions,
                lineInfo);




    lastNumPts = currentPoints.size();


    float* ptr = currentPoints[0].getPtr();


    vbo->bind();
    vbo->setAttributeData(shader.getAttributeLocation("position"),
                         ptr,  3, currentPoints.size(), GL_DYNAMIC_DRAW, sizeof(ofPoint));
    vbo->setAttributeData(shader.getAttributeLocation("previous"),
                         prevPositions[0].getPtr(),  3, prevPositions.size(), GL_DYNAMIC_DRAW, sizeof(ofPoint));
    vbo->setAttributeData(shader.getAttributeLocation("next"),
                         nextPositions[0].getPtr(),  3, nextPositions.size(), GL_DYNAMIC_DRAW, sizeof(ofPoint));

    vbo->setAttributeData(shader.getAttributeLocation("lineInfo"),
                         lineInfo[0].getPtr(),  3, lineInfo.size(), GL_DYNAMIC_DRAW, sizeof(ofPoint));

    vbo->setVertexData(&currentPoints[0], currentPoints.size(), GL_DYNAMIC_DRAW);
    //vbo.setTexCoordData(&texCoords[0], texCoords.size(), GL_DYNAMIC_DRAW);


    //checkError("e");

    //ofDisableDepthTest();
    //ofEnableAlphaBlending();
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    //ink.bind();


    ofSetColor(255);


    vbo->draw(GL_TRIANGLE_STRIP, 0, currentPoints.size());

    vbo->unbind();
    //checkError("f");
    shader.end();

    wholeScreen.end();

    //06-12 21:14:23.753: I/(7795): /storage/emulated/0/Android/data/cc.openframeworks.accelDraw/files/


   // screen.loadScreenData(0,0, ofGetWidth(), ofGetHeight());

    if (bRecordingMode == true){



    	subscreen.begin();
    	ofSetColor(255);
    	wholeScreen.draw(0,ofGetHeight()/4,ofGetWidth()/4, -ofGetHeight()/4);
    	subscreen.end();
    	subscreen.readToPixels(subscreenPixels);

    	unsigned char * temp  = subscreenPixels.getPixels();
		for (int i = 0; i < subscreenPixels.getWidth() * subscreenPixels.getHeight(); i++){
			data[i*3] = temp[i * 4];
			data[i*3+1] = temp[i * 4];
			data[i*3+2] = temp[i * 4];
		}

    	gifEncoder.addFrame(
    			data,
				 subscreenPixels.getWidth(),
				 subscreenPixels.getHeight(),
				 24,
    	                        1/20.0f
    	                        );

    }


    ofSetColor(255);
    wholeScreen.draw(0,ofGetHeight(), ofGetWidth(), -ofGetHeight());


    //checkError("g");


    //menu.draw(0,0);

//
//

//
    //checkError("h");


    menu.draw( (-menu.getWidth() + 250) * (1-menuOpenPct),ofGetHeight()-150);

}


void ofApp::handleMenuPress(ofPoint pt, bool bDrag){


	ofRectangle camRect(50-32, 75-32, 64,64);
	ofRectangle infoImg(250-32, 75-32, 64,64);
	ofRectangle lightningImg(450-32, 75-32, 64,64);
	ofRectangle triangle(650-32, 75-32, 920-650, 64);
	ofRectangle arrowImg(1050-32,75-32, 64, 64);

	camRect.scaleFromCenter(1.2);
	infoImg.scaleFromCenter(1.2);
	lightningImg.scaleFromCenter(1.2);
	triangle.scaleFromCenter(1.2);
	arrowImg.scaleFromCenter(1.2);

	if (!bDrag){
	if (arrowImg.inside(pt)){
		bMenuOpen = false;
	}

	if (lightningImg.inside(pt)){

		bLightningMode = !bLightningMode;
	}

	if (camRect.inside(pt)){

		gifEncoder.reset();

		bRecordingMode = true;
		recordingStartTime = ofGetElapsedTimeMillis();
	}
	}

	if (triangle.inside(pt)){
		triPct = 1.0 - (pt - triangle.getPosition()).x / (float)triangle.getWidth();
		triangleChangeEnergy = 1;
	}


}




//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::touchDown(int x, int y, int id){





	if (id == 0){

		bIsThisMouseEventInMenu = false;

		if (bMenuOpen == false && menuOpenPct < 0.01){
			ofPoint mousePt(x,y);
			ofRectangle rect((-menu.getWidth() + 250) * (1-menuOpenPct),ofGetHeight()-150, menu.getWidth(), menu.getHeight());
			if (rect.inside(mousePt)){
				bIsThisMouseEventInMenu = true;
				bMenuOpen = true;
			}
		}

		if (bMenuOpen == true && menuOpenPct > 0.95){
				ofPoint mousePt(x,y);
				ofRectangle rect((-menu.getWidth() + 250) * (1-menuOpenPct),ofGetHeight()-150, menu.getWidth(), menu.getHeight());
				if (rect.inside(mousePt)){
					bIsThisMouseEventInMenu = true;
					handleMenuPress(mousePt - rect.getPosition(), false );
				}
		}


		if (!bIsThisMouseEventInMenu){
			line.line.addVertex(ofPoint(x,y));
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
					}
				} else {
					line.line.addVertex(ofPoint(x,y));
				}
				line.computeWidths();
			} else {

				if (bMenuOpen == true && menuOpenPct > 0.95){
								ofPoint mousePt(x,y);
								ofRectangle rect((-menu.getWidth() + 250) * (1-menuOpenPct),ofGetHeight()-150, menu.getWidth(), menu.getHeight());
								if (rect.inside(mousePt)){
									bIsThisMouseEventInMenu = true;
									handleMenuPress(mousePt - rect.getPosition(), true );
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
			   line.computeWidths();

			   lines.push_back(line);
		   }
		    line.line.clear();
			}
	    }
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(int x, int y, int id){

	line.line.clear();
	lines.clear();

	//bMenuOpen = !bMenuOpen;

}

//--------------------------------------------------------------
void ofApp::touchCancelled(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

//--------------------------------------------------------------
void ofApp::pause(){

}

//--------------------------------------------------------------
void ofApp::stop(){

}

//--------------------------------------------------------------
void ofApp::resume(){

}

//--------------------------------------------------------------
void ofApp::reloadTextures(){

	bNeedToReallocateOpengl = true;
	nFramesToReallocate = 0;


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
