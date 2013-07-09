#pragma once

#include "ofMain.h"

#ifdef TARGET_RASPBERRY_PI

//  RASPBERRY-CAM
//  -----------------------------
//
//  Based on Jason Van Cleave code:
//  https://github.com/jvcleave/omxCameraApp
//
#include "ofxOMXVideoGrabber.h"

//  PhotoSensor
//  -----------------------------
//
//  Based on by jason van cleave on 3/9/13 wich is a translation of
//  http://learn.adafruit.com/reading-a-analog-in-and-controlling-audio-volume-with-the-raspberry-pi/connecting-the-cobbler-to-a-mcp3008
//  and http://raspberrypihobbyist.blogspot.com/2012/12/analog-interface.html
//
//  How to install wiringPi at:
//  https://github.com/openFrameworks-RaspberryPi/openFrameworks/wiki/Raspberry-Pi-Using-the-GPIO-pins-with-Wiring-Pi-and-openFrameworks
//
//  We will use WiringPi Pins 0 ( BCM 17 / GPIO0 )
//  and 3 ( BCM 22 / GPIO3 ) for back (<<) and forwards (>>)
//  https://projects.drogon.net/raspberry-pi/wiringpi/pins/
//
//  Diagram for simple switches at:
//  http://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/robot/buttons_and_switches/
//
#include "wiringPi.h"
#include "AnalogIn.h"
#endif

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
	
#ifdef TARGET_RASPBERRY_PI
    ofxOMXVideoGrabber cam;
	vector<OMX_IMAGEFILTERTYPE> imageFilters;
    int imageFiltersCounter;
#else
    ofVideoGrabber  cam;
#endif
	
};

