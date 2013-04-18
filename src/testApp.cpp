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
	vidGrabber.initGrabber(camWidth,camHeight,false);    
    actual.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
    
    bRec = false;
    
    bRequest = true;
    nDir = 1;
    nFrame = -1;
    nFrameMax = 0;
}


//--------------------------------------------------------------
void testApp::update(){
    
	vidGrabber.update();
	
	if ( nDir > 0) {
        // >>
        //
        
        if (nFrame == nFrameMax-1){
            //  Rec
            //
            if (bRequest){
                if (vidGrabber.isFrameNew() ){
                    
                    int w = vidGrabber.getWidth();
                    int h = vidGrabber.getHeight();
                    int nPixels = w*h;
                    
                    unsigned char * pixels = actual.getPixels();
                    unsigned char * pixelsRGB = vidGrabber.getPixels();
                    for(int i = 0; i < nPixels; i++){
                        pixels[i] = pixelsRGB[i*3]; //red
                    }
                    actual.setFromPixels(pixels, w, h, OF_IMAGE_GRAYSCALE);
                    bRequest = false;
                    bRec = true;
                    
                    cout << ofGetFrameNum() << " Request" << endl;
                }
                
            } else {
                nFrame++;
                actual.saveImage(ofToString(nFrame)+".jpg", OF_IMAGE_QUALITY_MEDIUM);
                nFrameMax++;
                bRec = false;
                nDir = 0;
                
                cout << ofGetFrameNum() << " SAVED" << endl;
            }
            
            
        } else {
            //  Play
            //
            nFrame++;
            actual.loadImage(ofToString(nFrame)+".jpg");
            nDir = 0;
        }
        
    } else if ( nDir < 0){
        // <<
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
