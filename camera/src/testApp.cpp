#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
	ofSetVerticalSync(true);
	ofSetFrameRate(30);

	int camWidth = 320*2;
	int camHeight = 240*2;
    int camFps = 15;
    
#ifdef TARGET_RASPBERRY_PI 
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
	pipeline << "v4l2src name=video_source device=/dev/video0 ! video/x-raw-yuv, ";// video/x-raw-rgb,";
	pipeline << "width="		<< camWidth	 <<	",";
	pipeline << "height="		<< camHeight 	<<	",";
	pipeline << "framerate="	<< camFps	 <<	"/1 ! ffmpegcolorspace ";
    
	bool didStart = cam.setPipeline(pipeline.str(), 8, false, camWidth, camHeight);
	if(didStart){
		ofLogVerbose() << "set pipeline SUCCESS";
	} else {
		ofLogError() << "set pipeline FAIL, pipeline: " << pipeline.str();
	}
    
	cam.play();
#else
	cam.setDesiredFrameRate(camFps);
	cam.setPixelFormat(OF_PIXELS_MONO);
    cam.initGrabber(camWidth,camHeight,false);
#endif

    actual.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
    
    bRec = false;
    bLigth = false;
    bRequest = false;
    
    nDir    = 0;
    nFrame  = 0;
    nFrameMax = 0;
    
    isReady = analogIn.setup();
}


//--------------------------------------------------------------
void testApp::update(){
    cam.update();
#ifdef TARGET_RASPBERRY_PI
    // Check for PINS
    //
    if (nDir == 0){
        if ( digitalRead(0) != 0 ){
            nDir = 1;
            if (nFrame == nFrameMax-1)
                bRequest = true;
        } else if ( digitalRead(3) != 0  ){
            nDir = -1;
        }
    }
#endif
    
    if (analogIn.value > 512){
        bLigth = true;
    } else {
        bLigth = false;
    }
    
    if (nDir != 0){
        
        if (bLigth){
            moveHeader();
            actual.loadImage(ofToString(nFrame)+".jpg");
            nDir = 0;
        } else {
            
            if (!bRequest){
                bRequest = true;
                requestNewFrame();
            } else if (bRec){
                
                if (nFrame < nFrameMax-1)
                    nFrameMax++;
                
                moveHeader();
                actual.saveImage(ofToString(nFrame)+".jpg", OF_IMAGE_QUALITY_MEDIUM);
                bRec = false;
                nDir = 0;
            }
        }
    }
    
}

void testApp::moveHeader(){
    if (nDir > 0){
        if (nFrame < nFrameMax-1)
            nFrame++;
    } else if (nDir < 0){
        if (nFrame > 0)
            nFrame--;
    }
}

void testApp::requestNewFrame(){
    
    //  Request the new pixels to the camera
    //
    if ( cam.isFrameNew() ){
        
        int w = cam.getWidth();
        int h = cam.getHeight();
        int nPixels = w*h;
        
        unsigned char * pixels = actual.getPixels();
        unsigned char * pixelsRGB = cam.getPixels();
        for(int i = 0; i < nPixels; i++){
#ifdef TARGET_RASPBERRY_PI
            pixels[i] = pixelsRGB[i];
#else
            pixels[i] = pixelsRGB[i*3]; // take the red channel
#endif
        }
        actual.setFromPixels(pixels, w, h, OF_IMAGE_GRAYSCALE);
        
        bRequest = false;
        bRec = true;
    }
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
    
    int colorValue = 255;
    
    stringstream info;
    info << "Fps: "         << ofGetFrameRate() << "\n";
    info << "Frame: "       << nFrame << "/" << nFrameMax << "\n";
	info << "Direction: "   << nDir                     << "\n";
	info << "Light Value: " << analogIn.value			<< "\n";
    ofDrawBitmapStringHighlight(info.str(), 15, 15, ofColor::black, ofColor::yellow);
    
#ifdef TARGET_RASPBERRY_PI
    colorValue = ofMap(analogIn.value, 0, 1024, 0, 255, true);
#endif
	
    ofSetColor(colorValue);
	actual.draw(ofGetWidth()*0.5 - actual.width*0.5, ofGetHeight()*0.5 - actual.height*0.5);
    
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
        } else if (key == OF_KEY_LEFT){
            nDir = -1;
        }
    }
    
    if (key == 'l'){
        analogIn.value = 1024;
    } else if (key == 'o'){
        analogIn.value = 20;
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
