#pragma once
#include "ofMain.h"



class lineWithInfo {
public:
	ofPolyline line;
    vector < ofPoint > widthInfo;       // x = direction * offset, y = distance, z = capPct
	vector < float > widths;
	void computeWidths();

//	ofPolyline origLine;
//	void addNoise(){
//
//		line.clear();
//		for (int i = 0; i < origLine.size(); i++){
//
////			float xAdd = ofSignedNoise(i/30.0 + 1000.0, ofGetElapsedTimef() * 5);
////			float yAdd = ofSignedNoise(i/30.0 + 10000.0, ofGetElapsedTimef() * 5);
////			float zAdd = ofSignedNoise(-i/30.0 + 1000.0, ofGetElapsedTimef() * 5);
////
////			float scale = sin(ofGetElapsedTimef()*10 + i / 50.0) * 0.5 + 0.5;
//			float pct = sin(ofGetElapsedTimef()) * 0.5 + 0.5;
//			if (i < powf(pct,0.1) * origLine.size()){
//				line.getVertices().push_back(origLine[i]); // + ofPoint(xAdd, yAdd, zAdd) * 100.0 * scale;
//			}
//		}
//
//
//	}

	bool bAmILightning;

};
