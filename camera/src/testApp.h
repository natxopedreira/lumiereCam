#pragma once

#include "ofMain.h"

#ifdef TARGET_RASPBERRY_PI
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
#endif

#include "AnalogIn.h"

#include "ofxOsc.h"
#define PORT 12345

#ifdef TARGET_RASPBERRY_PI
#define USE_GST
#endif

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

    void loadLUT(string _lutFile);
    void processState();
    void recordNewFrame();
    bool requestNewFrame();
    
#ifdef USE_GST
    ofGstVideoUtils cam;
#else
    ofVideoGrabber  cam;
#endif
    
    ofShader        shader;
    ofTexture       lutTex;
    
    ofImage         actual;
    int nFrameMax;  // number of max frame
    int nFrame;     // frame head
    
    ofxOscReceiver receiver;
    int nState;     // 0=(!A !B) 1=(A !B) 2=(A B)  3=(!A B)
    int nPreState;  //
    
    AnalogIn        analogIn;
    
    int             width,height;
    bool            bPlayMode,bFrameRecorded;
};
