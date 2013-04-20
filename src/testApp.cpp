#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
	ofSetVerticalSync(true);
	ofSetFrameRate(30);

	
	int camWidth = 320*2;
	int camHeight = 240*2;
    int camFps = 15;
    
#ifndef TARGET_OSX
    //  Setup WiringPi
    //
    if(wiringPiSetup() == -1){
        printf("Error on wiringPi setup\n");
    }
    
    pinMode(0,INPUT);
    pinMode(3,INPUT);
    
	//optimized pipeline for the PS3Eye
    //
	cam.allocate(camWidth,camHeight,8);
	stringstream pipeline;
	pipeline << "v4l2src name=video_source device=/dev/video0 ! video/x-raw-rgb,";
	pipeline << "width="		<< camWidth	 <<	",";
	pipeline << "height="		<< camHeight 	<<	",";
	pipeline << "framerate="	<< camFps	 <<	"/1 ";
    
	bool didStart = cam.setPipeline(pipeline.str(), 24, false, camWidth, camHeight);
	if(didStart){
		ofLogVerbose() << "set pipeline SUCCESS";
	} else {
		ofLogError() << "set pipeline FAIL, pipeline: " << pipeline.str();
	}
    
	cam.play();
    
    //  Or if don't use gstreamer
    //
//    cam.setDeviceID(1);
#else
	cam.setDesiredFrameRate(camFps);
	cam.initGrabber(camWidth,camHeight,false);
#endif

    actual.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
    
    bRec = false;
    bRequest = true;
    nDir = 1;
    nFrame = -1;
    nFrameMax = 0;
}


//--------------------------------------------------------------
void testApp::update(){
	cam.update();
    
#ifndef TARGET_OSX
    // Check for PINS
    //
    if (nDir == 0){
        if ( digitalRead(0) != 0 ){
            cout << "PIN 0 = " << digitalRead(0) << endl;
            nDir = 1;
            if (nFrame == nFrameMax-1)
                bRequest = true;
        } else if ( digitalRead(3) != 0  ){
            cout << "PIN 3 = " << digitalRead(3) << endl;
            nDir = -1;
        }
    }
#endif
	
	if ( nDir > 0) {
        // >>
        //
        
        if (nFrame == nFrameMax-1){
            
            //  REC
            //
            //  The recording is made in two stapes ( one in a frame and the other in the next one)
            //  In order to distribute the work and make things easyier for the CPU
            //
            if (bRequest){
                
                //  Request the new pixels to the camera
                //
                if ( cam.isFrameNew() ){
                    
                    int w = cam.getWidth();
                    int h = cam.getHeight();
                    int nPixels = w*h;
                    
                    unsigned char * pixels = actual.getPixels();
                    unsigned char * pixelsRGB = cam.getPixels();
                    for(int i = 0; i < nPixels; i++){
                        pixels[i] = pixelsRGB[i*3]; //red
                    }
                    actual.setFromPixels(pixels, w, h, OF_IMAGE_GRAYSCALE);
                    bRequest = false;
                    bRec = true;
                }
                
            } else {
                
                //  Save the pixels into an image
                //
                nFrame++;
                actual.saveImage(ofToString(nFrame)+".jpg", OF_IMAGE_QUALITY_MEDIUM);
                nFrameMax++;
                bRec = false;
                nDir = 0;
            }
            
            
        } else {
            
            //  PLAY
            //
            //  Load the next frame
            //
            nFrame++;
            actual.loadImage(ofToString(nFrame)+".jpg");
            nDir = 0;
        }
        
    } else if ( nDir < 0){
        // <<
        //
        
        //  Load previus frame
        //
        if ( nFrame > 0){
            nFrame--;
            actual.loadImage(ofToString(nFrame)+".jpg");
        }
        nDir = 0;
    }
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
		
    ofSetColor(255);
	actual.draw(ofGetWidth()*0.5 - actual.width*0.5, ofGetHeight()*0.5 - actual.height*0.5);
    
    ofDrawBitmapString("Frame: " + ofToString(nFrame) + "/" + ofToString(nFrameMax), 15,15);
    ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate()), 15,30);
    
    if ( bRec ){
        ofSetColor(255,0,0);
        ofCircle( ofGetWidth()*0.5 - actual.width*0.5 + 20, ofGetHeight()*0.5 - actual.height*0.5 + 20, 10);
    }
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
    
    if (nDir == 0){
        if (key == OF_KEY_RIGHT){
            nDir = 1;
            if (nFrame == nFrameMax-1)
                bRequest = true;
        } else if (key == OF_KEY_LEFT){
            nDir = -1;
        }
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
