#include "line.h"
#include "ofApp.h"

void lineWithInfo::computeWidths(){
    
    widthInfo.clear();

    ofApp * appPtr = (ofApp *) ofGetAppPtr();
    float minWidth = appPtr->minWidth;
    float maxWidth = appPtr->maxWidth;
    bAmILightning = appPtr->bLightningMode;
    
	float totalDistance = 0;
	float avgSpeed = 0;
    
    if (line.size() < 2) return;
    
	for (int i = 0; i < line.size()-1; i++){
		float len = (line[i] - line[i+1]).length();
		totalDistance += len;
		avgSpeed += len;
	}
	avgSpeed /= MAX(1.0, line.size());
    
	float capLength = 60;
    
    float widthSmooth = minWidth;
    float runningDistance = 0;
    for (int i = 0; i < line.size(); i++){
        
        float scalePct = 1;
        float scalePctForTex = 1;
        
        if (runningDistance < capLength){
            scalePct = runningDistance/capLength;
            scalePctForTex = scalePct;
        } else if ( (totalDistance - runningDistance) < capLength){
            scalePct =  ((totalDistance - runningDistance)/capLength);
            scalePctForTex =scalePct * -1;
        }
        int i_p_1 = i + 1;
        if (i_p_1 > line.size()-1) i_p_1 = line.size()-1;
        float len = (line[i] - line[i_p_1 ]).length();
        
        
        float widthPct = ofMap(len, 1,50, 0, 1, true);;
        widthSmooth = 0.85f * widthSmooth + 0.15 * ofMap(powf(widthPct, 3.4), 0, 1, minWidth, maxWidth, true);
        
        
        ofPoint pt;
        pt.x = widthSmooth * scalePct;
        pt.y = runningDistance;
        pt.z = scalePctForTex;
        
        widthInfo.push_back(pt * ofPoint(-1,1,1));
        widthInfo.push_back(pt);
        
        
        runningDistance += len;
        
    }
    
}

