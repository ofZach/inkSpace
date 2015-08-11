#include "introScreen.h"


void introScreen::setupTitle(){
    
	introScreenEnergyTarget = 1.0;
	titlePts.clear();
    
	ofPoint pos(screenWidth/2, screenHeight/2);
	float w = 500;
	float h = 500;
    
    
	titlePts.push_back(pos + ofPoint(-w/2, -h/2)*1.6);
	titlePts.push_back(pos + ofPoint(w/2, -h/2)*1.6);
	titlePts.push_back(pos + ofPoint(-w/2, h/2)*1.6);
	titlePts.push_back(pos + ofPoint(w/2, h/2)*1.6);
    
	for (int i = 0; i < 4; i++){
		titlePts[i].z -= 200;
	}
    
	linesForIntroScreen.clear(); 	// <--- noooooo!
    
    
    string file = infoScreenDir.getPath( (int)ofRandom(0,infoScreenDir.size() + 100) % infoScreenDir.size());
    ofxXmlSettings settings;
    settings.load(file);
    
    settings.pushTag("drawing");
    int nLines = settings.getNumTags("line");
    for (int i = 0; i < nLines; i++){
        settings.pushTag("line", i);
        
        lineWithInfo temp;
        
        temp.bAmILightning = settings.getValue("bAmILightning", 0) == 1 ? true : false;
        temp.startTimeMillis = settings.getValue("startTimeMillis", 0);
        temp.durationMillis = settings.getValue("durationMillis", 0);
        
        int currentMillis = ofGetElapsedTimeMillis();
        while (temp.startTimeMillis > currentMillis){
            temp.startTimeMillis -= temp.durationMillis;
            
        }
        
        
        settings.pushTag("pts");
        int nPts = settings.getNumTags("pt");
        for (int j = 0; j < nPts; j++){
            
            settings.pushTag("pt", j);
            float x = settings.getValue("x", 0.0);
            float y = settings.getValue("y", 0.0);
            float z = settings.getValue("z", 0.0);
            float wx = settings.getValue("wx", 0.0);
            float wy = settings.getValue("wy", 0.0);
            float wz = settings.getValue("wz", 0.0);
            int offset = settings.getValue("offset", 0);
            
            ofPoint tempPt = ofPoint(x,y,z);
            tempPt -= ofPoint(540,888);
            tempPt += ofPoint(screenWidth/2.0, screenHeight/2.0);
            temp.line.addVertex(tempPt);
            temp.widthInfo.push_back( ofPoint(wx, wy, wz));
            temp.widthInfo.push_back( ofPoint(-wx, wy, wz));
            temp.timeOffsetMillis.push_back(offset);
            
            settings.popTag();
        }
        
        if (temp.bAmILightning){
            temp.playbackPos =  temp.timeOffsetMillis[temp.timeOffsetMillis.size()-1];
            temp.bAmDoneDrawing = true;
        }
        
        linesForIntroScreen.push_back(temp);
        
        
        
        settings.popTag();
        
        
        
        settings.popTag();
        
    }
    
}


void introScreen::drawTitle(){
    
    
	ofPoint midPt(screenWidth/2, screenHeight/2);
	ofMatrix4x4 worldFlipMatrix;
    worldFlipMatrix.makeScaleMatrix(1,-1,1);
    
    ofPoint tempPts[4];
    for (int i = 0; i < 4; i++){
    	tempPts[i] = titlePts[i] - midPt;
    	tempPts[i] = tempPts[i] * worldFlipMatrix;
    	tempPts[i] += midPt;
        
    }
    
    
	ofMesh temp;
	temp.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    
	temp.addVertex(tempPts[0]);
	temp.addTexCoord(ofPoint(0,0));
	temp.addVertex(tempPts[1]);
	temp.addTexCoord(ofPoint(1,0));
	temp.addVertex(tempPts[2]);
	temp.addTexCoord(ofPoint(0,1));
	temp.addVertex(tempPts[3]);
	temp.addTexCoord(ofPoint(1,1));
    
	title.bind();
	temp.draw();
	title.unbind();
    
    
}