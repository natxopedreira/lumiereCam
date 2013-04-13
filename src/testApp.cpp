#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	
	int camWidth = 640;
	int camHeight = 480;

	vidGrabber.setVerbose(false);
	vidGrabber.setDeviceID(1);
	vidGrabber.setDesiredFrameRate(15);
	vidGrabber.setUseTexture(true);
	vidGrabber.initGrabber(camWidth,camHeight);
	
	videoTexture.allocate(camWidth,camHeight, GL_RGB);	
}


//--------------------------------------------------------------
void testApp::update(){
	
	vidGrabber.update();
	
	if (vidGrabber.isFrameNew()){
		
	}

}

//--------------------------------------------------------------
void testApp::draw(){

	ofBackground(100,100,100);
		
        ofSetColor(255);
	vidGrabber.draw(0,0,ofGetScreenWidth(),ofGetScreenHeight());
	//videoTexture.draw(20+camWidth,20,camWidth,camHeight);
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
