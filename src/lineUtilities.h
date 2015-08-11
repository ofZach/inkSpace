

#include "ofMain.h"
#include "line.h"


// I take a "line" and put data into some vectors

void addPolyline ( 	lineWithInfo & line,
                  vector < ofPoint > & currentPoints,
                  vector < ofPoint > & prevPositions,
                  vector < ofPoint > & nextPositions,
                  vector < ofPoint > & lineInfo,
                  bool bAddExtraStartPoints,
                  bool bAddExtraEndPoints,
                  float addPct = 1.0);


void splitPolylineAt( ofPolyline & a,  ofPolyline & b,  ofPolyline & c, int pos);
void splitPolylineAt( ofPolyline & a,  ofPolyline & b,  ofPolyline & c,  ofPolyline & d, int posA, int posB);
void joinPolyline( ofPolyline & a,  ofPolyline & b,  ofPolyline & c);
void joinPolyline( ofPolyline & a,  ofPolyline & b,  ofPolyline & c, ofPolyline & d);
