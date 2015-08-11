#pragma once

#include "ofMain.h"
#include "line.h"

class introScreen {
    
public:

    
    int screenWidth, screenHeight;
    float introScreenEnergy;
    float introScreenEnergyTarget;
    vector < lineWithInfo > linesForIntroScreen;
    void setupTitle();
    void drawTitle();
    ofImage title;
    vector < ofPoint > titlePts;
    ofDirectory infoScreenDir;
    
    
};