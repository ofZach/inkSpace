#include "lineUtilities.h"


// todo: add first and last line things... (no degenerate first and last points)


float doubleExponentialSigmoid (float x, float a){

  float epsilon = 0.00001;
  float min_param_a = 0.0 + epsilon;
  float max_param_a = 1.0 - epsilon;
  a = MIN(max_param_a, MAX(min_param_a, a));
  a = 1.0-a; // for sensible results

  float y = 0;
  if (x<=0.5){
    y = (powf(2.0*x, 1.0/a))/2.0;
  } else {
    y = 1.0 - (powf(2.0*(1.0-x), 1.0/a))/2.0;
  }
  return y;
}

void addPolyline ( 	lineWithInfo & line,
                  vector < ofPoint > & currentPoints,
                  vector < ofPoint > & prevPositions,
                  vector < ofPoint > & nextPositions,
                  vector < ofPoint > & lineInfo,
                  bool bAddExtraStartPoints,
                  bool bAddExtraEndPoints,
                  float addPct){

    if (line.widthInfo.size() <= 2) return;
    
    int startPos = currentPoints.size();
    float distance = 0;
    
    int lineSize = line.line.size();

    if (addPct > 0.99) addPct = 1.01;

    bool bAddEndThisTime = false;
    bool bSkipPt = false;

    for (int i = 0; i < (line.line.size()); i++){
        
    	float pctTemp = ofMap(i, 0, (line.line.size()-1), 0, 1, true);
    	if (pctTemp > addPct){
    		if (bSkipPt == false){
    			bAddEndThisTime = true;
    		}
    		bSkipPt = true;
    	}

    	float widthScaler = 1.0;
    	if (addPct < 1.0){
    		float pctOfPct = pctTemp / addPct;
    		float taperPoint = ofMap(addPct, 0, 1, 0.8, 0.97);
    		float widthScalerTemp = ofMap(pctOfPct,taperPoint, 1.0, 1.0, 0.0, true);

    		widthScaler = ofLerp(widthScalerTemp, 1.0, addPct*addPct);
    	}



        int iGood = i;
        if (iGood < 0) iGood = 0;
        if (iGood >= line.line.size()) iGood = line.line.size()-1;
        //int j = iGood;
        // todo: repeat 0 and size() - 1
        int i_m_1 = (iGood - 1) < 0 ? iGood : iGood -1 ;
        int i_p_1 = (iGood + 1) > (line.line.size()-1) ? iGood : iGood + 1 ;
        float pct = iGood / (float)line.line.size();
        
        //distance += (line.line[iGood] - line.line[i_m_1]).length();
        
        for (int j = 0; j < 2; j++){
            
            ofPoint pt = line.widthInfo[i * 2 + j] ;
        	
            if (line.bAmILightning){ 
            	int time = line.timeOffsetMillis[i];
            	int totalTime = MAX(line.durationMillis, 1000) + 1000;
                int playbackPos = line.playbackPos;
                int diff = (playbackPos - time);
                // diff = 
                //if (line.bAmDoneDrawing == true){
                if (diff > totalTime/2){
                    playbackPos -= totalTime;
                }
                if (diff < -totalTime/2){
                    playbackPos += totalTime;
                }
                //}
                //ofLog() << "playbackPos: " <<  playbackPos << endl;
                float realDiff = fabs(playbackPos - time) / (totalTime/2.0);
                //ofLog() << realDiff << endl;
                float pct = 0;

                float timeToCheck = MAX(MIN(1.0, (line.durationMillis/1000.0)/2), 0.2); //MAX(line.timeToCheck / 1000.0, 0.2);


                if (realDiff <= timeToCheck){
                		pct = 1.0 - (float)(realDiff) / timeToCheck;
                		pct = (pct - 0.5) * 2 + 0.5;

                		if (pct > 1) pct = 1.0;
                		if (pct < 0) pct = 0.0;
                		//pct = doubleExponentialSigmoid(pct, 0.727);
//                    if (pct < 0) pct = 0;
//                    if (pct > 1) pct = 1;
//                    float shape = sin(pct*PI);
//                    pct = powf(shape, 0.8);
                }

                pt.x *= pct;




            }

            pt.x *= widthScaler;

            //    pt.x *=  (1.0 * powf((sin(-(i/20.0) + ofGetElapsedTimef() * 8) * 0.5 + 0.5), 10.0));
            
        	//if (j == 0) pt.x = -170;
        	//else pt.x = 170;
            if (!bSkipPt){
            	lineInfo.push_back( pt );

            	currentPoints.push_back(line.line[iGood]);
            	prevPositions.push_back(line.line[i_m_1]);
            	nextPositions.push_back(line.line[i_p_1]);
            }
            
            // add extra points at the start and end to make "degenerate" triangles between lines.
            // repeat first and last to "disconnect" lines from the next.
            // this is per https://developer.apple.com/library/ios/documentation/3DDrawing/Conceptual/OpenGLES_ProgrammingGuide/TechniquesforWorkingwithVertexData/TechniquesforWorkingwithVertexData.html

            if (bAddExtraStartPoints){
                if (i == 0 && j == 0){
                    currentPoints.push_back(currentPoints[startPos]);
                    prevPositions.push_back(prevPositions[startPos]);
                    nextPositions.push_back(nextPositions[startPos]);
                    lineInfo.push_back(lineInfo[startPos]);
                    
                    currentPoints.push_back(currentPoints[startPos]);
                    prevPositions.push_back(prevPositions[startPos]);
                    nextPositions.push_back(nextPositions[startPos]);
                    lineInfo.push_back(lineInfo[startPos]);
                    
                }
            }
            
            if (bAddExtraEndPoints){
                if (i == line.line.size()-1 && j == 1){
                    currentPoints.push_back(currentPoints[currentPoints.size()-1]);
                    prevPositions.push_back(prevPositions[prevPositions.size()-1]);
                    nextPositions.push_back(nextPositions[nextPositions.size()-1]);
                    lineInfo.push_back(lineInfo[lineInfo.size()-1]);
                    
                    currentPoints.push_back(currentPoints[currentPoints.size()-1]);
                    prevPositions.push_back(prevPositions[prevPositions.size()-1]);
                    nextPositions.push_back(nextPositions[nextPositions.size()-1]);
                    lineInfo.push_back(lineInfo[lineInfo.size()-1]);
                    
                }
            }

            if (bAddEndThisTime == true && i > 0){

            	currentPoints.push_back(currentPoints[currentPoints.size()-1]);
            	prevPositions.push_back(prevPositions[prevPositions.size()-1]);
            	nextPositions.push_back(nextPositions[nextPositions.size()-1]);
            	lineInfo.push_back(lineInfo[lineInfo.size()-1]);

            	currentPoints.push_back(currentPoints[currentPoints.size()-1]);
            	prevPositions.push_back(prevPositions[prevPositions.size()-1]);
            	nextPositions.push_back(nextPositions[nextPositions.size()-1]);
            	lineInfo.push_back(lineInfo[lineInfo.size()-1]);
            	bAddEndThisTime = false;
            }


        }
    }
    
    
}

    void splitPolylineAt( ofPolyline & a,  ofPolyline & b,  ofPolyline & c, int pos){

        if (a.size() < pos){
            return;
        }

        b.clear();
        c.clear();

        for (int i = 0; i < a.size(); i++){
            if (i < pos) b.addVertex(a[i]);
            else c.addVertex(a[i]);
        }


    }

    void splitPolylineAt( ofPolyline & a,  ofPolyline & b,  ofPolyline & c,  ofPolyline & d, int posA, int posB){


        if (a.size() < posA || a.size() < posB){
            return;
        }

        b.clear();
        c.clear();
        d.clear();

        for (int i = 0; i < a.size(); i++){
            if (i < posA) b.addVertex(a[i]);
            else if (i >= posA && i < posB) c.addVertex(a[i]);
            else d.addVertex(a[i]);
        }


    }



    void joinPolyline( ofPolyline & a,  ofPolyline & b,  ofPolyline & c){

        a.clear();

        for (int i = 0; i < b.size(); i++){
            a.addVertex(b[i]);
        }

        for (int i = 0; i < c.size(); i++){
            a.addVertex(c[i]);
        }

    }

    void joinPolyline( ofPolyline & a,  ofPolyline & b,  ofPolyline & c, ofPolyline & d){

        a.clear();

        for (int i = 0; i < b.size(); i++){
            a.addVertex(b[i]);
        }

        for (int i = 0; i < c.size(); i++){
            a.addVertex(c[i]);
        }

        for (int i = 0; i < d.size(); i++){
            a.addVertex(d[i]);
        }

    }



