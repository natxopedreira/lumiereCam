#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp{
public:
    
    void setup();
    void update();
    void draw();
		
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
		
    ofVideoGrabber  vidGrabber;
    
    ofImage         prev;
    ofImage         actual;
    
    int nFrameMax;  // number of max frame
    int nFrame;     // frame head
    
    int nDir;
    //  -1  backward
    //  0   no move
    //  1   forward
    
    bool    bRec,bRequest;
};
