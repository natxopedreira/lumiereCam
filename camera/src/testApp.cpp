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
    cam.update();
    actual.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
    
    //  Light Sensor
    //
    analogIn.setup();
    bPlayMode = false;
    bFrameRecorded = false;
    
    // listen on the given port
	receiver.setup(PORT);
    nState = nPreState = 0;
    
    nFrame  = 0;
    nFrameMax = 0;
}


//--------------------------------------------------------------
void testApp::update(){
    
#ifdef TARGET_RASPBERRY_PI
    // Check for PINS
    //
//    if (nDir == 0){
//        if ( digitalRead(0) != 0 ){
//            nDir = 1;
//            if (nFrame == nFrameMax-1)
//                bRequest = true;
//        } else if ( digitalRead(3) != 0  ){
//            nDir = -1;
//        }
//    }
#endif
    
    //  Light sensor
    //
    if (analogIn.value > 512) bPlayMode = true;
    else bPlayMode = false;
    
    //  A&B Sensors
	while(receiver.hasWaitingMessages()){
        ofxOscMessage m;
		receiver.getNextMessage(&m);
        
        // check for mouse moved message
        //
		if(m.getAddress() == "/AB"){
            bool bA,bB = false;
			bA = m.getArgAsInt32(0);
			bB = m.getArgAsInt32(1);
		
            //  Compute STATE
            //  https://raw.github.com/patriciogonzalezvivo/lumiereCam/master/images/disk.png
            //
            nPreState = nState;
            if      (!bA && !bB)nState = 0;
            else if (bA && !bB) nState = 1;
            else if (bA && bB)  nState = 2;
            else if (!bA && bB) nState = 3;
            
            processState();
        }
    }
}

void testApp::processState(){
    
    if ( (nState == 1) && (nPreState == 0) ){
        // <<--
        //
        if (bPlayMode){
            if (nFrame+1 < nFrameMax)
                nFrame++;
            
            actual.loadImage(ofToString(nFrame)+".jpg");
        } else {
            if (nFrame+1 >= nFrameMax )
                nFrameMax++;
            if (nFrame+1 < nFrameMax)
                nFrame++;
            
            requestNewFrame();
        }
        
        actual.update();
    } else if ( (nState == 2) && ( nPreState != 2)){
        //  ( 0 )
        //
        if (!bPlayMode){
            actual.saveImage(ofToString(nFrame)+".jpg", OF_IMAGE_QUALITY_MEDIUM);
            bFrameRecorded = true;
        }
        
    } else if ( (nState == 3) && ( nPreState == 0)){
        // -->>
        //
        
        if (bPlayMode){
            if (nFrame-1 >= 0)
                nFrame--;
            
            actual.loadImage(ofToString(nFrame)+".jpg");
        } else {
            if (nFrame+1 >= nFrameMax )
                nFrameMax++;
            if (nFrame+1 < nFrameMax)
                nFrame++;
            
            requestNewFrame();
        }
    }
}

void testApp::requestNewFrame(){
    
    cam.update();
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
//        cout << "Request frame OK" << endl;
    } else {
//        cout << "Request frame ERROR" << endl;
    }
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
    
    int colorValue = 255;
    
    stringstream info;
    info << "Fps: "   << ofGetFrameRate() << "\n";
    info << "Frame: " << nFrame << "/" << nFrameMax << "\n";
	info << "State: " << nState << "\n";
	info << "Light: " << analogIn.value << ((bPlayMode)?" (Play)":" (Rec)") << "\n";
    ofDrawBitmapStringHighlight(info.str(), 15, 15, ofColor::black, ofColor::white);
    
#ifdef TARGET_RASPBERRY_PI
    colorValue = ofMap(analogIn.value, 0, 1024, 0, 255, true);
#endif
	
    ofSetColor(colorValue);
	actual.draw(ofGetWidth()*0.5 - actual.width*0.5, ofGetHeight()*0.5 - actual.height*0.5);
    
    if ( bFrameRecorded ){
        ofSetColor(255,0,0);
        ofCircle( ofGetWidth()*0.5 - actual.width*0.5 + 20, ofGetHeight()*0.5 - actual.height*0.5 + 20, 10);
        bFrameRecorded = false;
    }
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
    
    if (key == ' '){
        if (bPlayMode){
            analogIn.value = 20;
        } else {
            analogIn.value = 1024;
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
