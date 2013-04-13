#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofEnableAlphaBlending();
	ofSetVerticalSync(true);
	
	int camWidth = 320*2;
	int camHeight = 240*2;

#ifndef TARGET_OSX
	vidGrabber.setDeviceID(1);
#endif
	vidGrabber.setDesiredFrameRate(15);
	vidGrabber.initGrabber(camWidth,camHeight,true);
    actual.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
    
    nDir = 1;
    nFrame = -1;
    nFrameMax = -1;
}


//--------------------------------------------------------------
void testApp::update(){
    
	vidGrabber.update();
	
	if ( nDir > 0 && nFrame == nFrameMax ){
        //  Record a new frame
        //
        actual.saveImage(ofToString(nFrameMax)+".jpg");
        actual.setFromPixels(vidGrabber.getPixelsRef());
        nDir = 0;
	} else if ( nDir != 0 ){
        
        //  Go back and fordward
        //
        actual.loadImage(ofToString(nFrame)+".jpg");
        nDir = 0;
    } 

}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
		
    ofSetColor(255);
	actual.draw(ofGetWidth()*0.5 - actual.width*0.5, ofGetHeight()*0.5 - actual.height*0.5);
    
    ofDrawBitmapString("Frame: " + ofToString(nFrame) + "/" + ofToString(nFrameMax), 15,15);
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
    if (key == OF_KEY_RIGHT){
        nDir = 1;
        nFrame++;
        if (nFrame > nFrameMax)
            nFrameMax = nFrame;
    } else if (key == OF_KEY_LEFT){
        nDir = -1;
        if (nFrame > 0)
            nFrame--;
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
