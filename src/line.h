#pragma once
#include "ofMain.h"
#include "ofxXmlSettings.h"


class lineWithInfo {
public:

	lineWithInfo(){
		bAmDoneDrawing = false;
	}


	void writeToXmlFile(ofxXmlSettings & settings){

		settings.addValue("bAmILightning", bAmILightning ? 1 : 0);
		settings.addValue("startTimeMillis", startTimeMillis);
		settings.addValue("durationMillis", durationMillis);
		settings.addTag("pts");
		settings.pushTag("pts");

		ofLog() << "line" << line.size() << " " << widthInfo.size() << " " << widths.size() << " " << timeOffsetMillis.size() << endl;

		for (int i = 0; i < line.size(); i++){
			settings.addTag("pt");
			settings.pushTag("pt", i);
			settings.addValue("x", line[i].x );
			settings.addValue("y", line[i].y );
			settings.addValue("z", line[i].z );
			settings.addValue("wx", widthInfo[i*2].x );
			settings.addValue("wy", widthInfo[i*2].y );
			settings.addValue("wz", widthInfo[i*2].z );

//			settings.addValue("wx2", widthInfo[i*2+1].x );	// this is just flipped.
//			settings.addValue("wy2", widthInfo[i*2+1].y );
//			settings.addValue("wz2", widthInfo[i*2+1].z );

			//settings.addValue("width", widths[i] );
			settings.addValue("offset", timeOffsetMillis[i] );
			settings.popTag();
		}

		settings.popTag();

	}


	int startTimeMillis;
	int durationMillis;
	vector < int > timeOffsetMillis;

	ofPolyline line;
    vector < ofPoint > widthInfo;       // x = direction * offset, y = distance, z = capPct
	vector < float > widths;
	void computeWidths();
	bool bAmILightning;
    

    
    void startDrawing(){
    	//timeToCheck = 0.2;
    	timeOffsetMillis.clear();
    	startTimeMillis = ofGetElapsedTimeMillis();
    	durationMillis = 0;
    	playbackPos = 0;
    }

    void addedVertex(){

    	timeOffsetMillis.push_back(   ofGetElapsedTimeMillis() - startTimeMillis );
    	durationMillis = ofGetElapsedTimeMillis() - startTimeMillis;
    }

    //------------------------------------------------------------
    void finishDrawing(){
    	durationMillis = ofGetElapsedTimeMillis() - startTimeMillis;
    	bAmDoneDrawing = true;
    	//ofLog() << "finished drawing " << line.size() << "," <<  timeOffsetMillis.size() << endl;

    }

    int lastFrameTime = 0;

    void update(){

    	if (timeOffsetMillis.size() > 0){
    	    	//timeToCheck = 0.98f * timeToCheck + 0.02 * timeOffsetMillis[timeOffsetMillis.size()-1] / 5.0;
    	    	//durationMillis = timeOffsetMillis[timeOffsetMillis.size()-1] - startTimeMillis + MAX(timeToCheck / 1000.0, 1.2)*1000;;

    	}


    	// if we don't have anything return;
    	if (line.size() == 0) return;

    	if (bAmDoneDrawing == true){
    		//ofLog() << "done drawing " << endl;
    		playbackPos += (int)(ofGetLastFrameTime() * 1000.0);
    		if (timeOffsetMillis.size() > 0){				// how could this possibly happen?  still check to be sure...
    			playbackPos %= (durationMillis + 1000);
    		}

    	} else {

    		//ofLog() << "not done drawing " << endl;
    		if (timeOffsetMillis.size() == 0) playbackPos = 0;
    		else playbackPos =  timeOffsetMillis[timeOffsetMillis.size()-1];

    	}

    	//ofLog() << " " << playbackPos << " ? " << endl;
    }

    float timeToCheck;
    int playbackPos;	// this should be the last point until I am done drawing
    bool bAmDoneDrawing;







};
