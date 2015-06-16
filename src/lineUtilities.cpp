#include "lineUtilities.h"



void addPolyline ( 	lineWithInfo & line,
                  vector < ofPoint > & currentPoints,
                  vector < ofPoint > & prevPositions,
                  vector < ofPoint > & nextPositions,
                  vector < ofPoint > & lineInfo){
    
    
    if (line.widthInfo.size() == 0) return;
    
    int startPos = currentPoints.size();
    float distance = 0;
    
    for (int i = 0; i < (line.line.size()); i++){
        
        // repeat first and last to "disconnect" lines from the next.
        // as per https://developer.apple.com/library/ios/documentation/3DDrawing/Conceptual/OpenGLES_ProgrammingGuide/TechniquesforWorkingwithVertexData/TechniquesforWorkingwithVertexData.html
        
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
            
            
            //cout << line.widthInfo[i * 2 + j] << endl;

        	ofPoint pt = line.widthInfo[i * 2 + j] ;
        	if (line.bAmILightning) pt.x *=  (1.0 * powf((sin(-(i/20.0) + ofGetElapsedTimef() * 8) * 0.5 + 0.5), 10.0));
            
            lineInfo.push_back( pt );
            
            currentPoints.push_back(line.line[iGood]);
            prevPositions.push_back(line.line[i_m_1]);
            nextPositions.push_back(line.line[i_p_1]);
            //if (j % 2 == 0) direction.push_back(1);
            //else direction.push_back(-1);
            
            
            if (i == 0 && j == 0){
                currentPoints.push_back(currentPoints[startPos]);
                //thickness.push_back( thickness[startPos]);
                prevPositions.push_back(prevPositions[startPos]);
                nextPositions.push_back(nextPositions[startPos]);
                //direction.push_back(direction[startPos]);
                lineInfo.push_back(lineInfo[startPos]);
                
                currentPoints.push_back(currentPoints[startPos]);
                //thickness.push_back( thickness[startPos]);
                prevPositions.push_back(prevPositions[startPos]);
                nextPositions.push_back(nextPositions[startPos]);
                //direction.push_back(direction[startPos]);
                lineInfo.push_back(lineInfo[startPos]);
                
                
            }
            
            if (i == line.line.size()-1 && j == 1){
                currentPoints.push_back(currentPoints[currentPoints.size()-1]);
                //thickness.push_back( thickness[thickness.size()-1]);
                prevPositions.push_back(prevPositions[prevPositions.size()-1]);
                nextPositions.push_back(nextPositions[nextPositions.size()-1]);
                //direction.push_back(direction[direction.size()-1]);
                lineInfo.push_back(lineInfo[lineInfo.size()-1]);
                
                currentPoints.push_back(currentPoints[currentPoints.size()-1]);
                //thickness.push_back( thickness[thickness.size()-1]);
                prevPositions.push_back(prevPositions[prevPositions.size()-1]);
                nextPositions.push_back(nextPositions[nextPositions.size()-1]);
                //direction.push_back(direction[direction.size()-1]);
                lineInfo.push_back(lineInfo[lineInfo.size()-1]);
                
            }
        }
    }
    
    
}
