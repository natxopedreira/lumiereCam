#pragma once

#include "ofMain.h"
#include "ofxOMXVideoGrabber.h"

//  Based on Jason Van Cleave code:
//  https://github.com/jvcleave/omxCameraApp
//

class testApp : public ofBaseApp{
public:

	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);	

	ofShader shader;
    bool doShader;
	
	ofxOMXVideoGrabber omxVideoGrabber;
	vector<OMX_IMAGEFILTERTYPE> imageFilters;
	
    int imageFiltersCounter;
};

