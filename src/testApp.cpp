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
	vidGrabber.initGrabber(camWidth,camHeight);
    
}


//--------------------------------------------------------------
void testApp::update(){
    
	vidGrabber.update();
	
	if (vidGrabber.isFrameNew()){
		
	}

}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
		
    ofSetColor(255);
	vidGrabber.draw(0,0);//,ofGetScreenWidth(),ofGetScreenHeight());
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 

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
