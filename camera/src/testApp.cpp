#include "testApp.h"
#define STRINGIFY(A) #A

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

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
#endif
    
#ifdef USE_GST
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
    cam.initGrabber(camWidth,camHeight,false);
#endif
    cam.update();
    actual.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
    
    //  Load Shader
    //
    //shader.load("", "oldFilm.fs");
    
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
    firstFrame = 0;
    
    width = ofGetWidth();
    height = ofGetHeight();
    
    bDebug = true;
    bNotUsed = true;
    analogIn.value = 0;
}


//--------------------------------------------------------------
void testApp::update(){
    
    //  Update Camera
    //------------------------------------------------
    //
    cam.update();
    
    //  Check Light sensor
    //------------------------------------------------
    //
    if (analogIn.value < 512){
        bPlayMode = true;
    }else{
    	bPlayMode = false;
        if(bPlayMode) bNotUsed = false;
    }
    
    //  Check disk state based on A&B Sensors
    //------------------------------------------------
    
    //  1. trough OSC ( testing  )
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
            if      (!bA && !bB)nState = 0;
            else if (bA && !bB) nState = 1;
            else if (bA && bB)  nState = 2;
            else if (!bA && bB) nState = 3;
        }
    }
    
    //  2. trough PINS (just raspbery)
#ifdef TARGET_RASPBERRY_PI
    bool bA,bB = false;
    
    if (digitalRead(0) != 0)
        bA = true;
    
    if (digitalRead(3) != 0)
        bB = true;
    
    //  Compute STATE
    //  https://raw.github.com/patriciogonzalezvivo/lumiereCam/master/images/disk.png
    //
    if      (!bA && !bB) nState = 0;
    else if (bA  && !bB) nState = 1;
    else if (bA  &&  bB) nState = 2;
    else if (!bA &&  bB) nState = 3;
#endif
    
    //  3. trough keys ( testing )
    if ( bNext ){
        nState = (nState+1)%4;
    } else if ( bPrev ){
        nState--;
        if (nState == -1)
            nState = 3;
    }
    
    //  Do what ever is need to do
    //------------------------------------------------
    //
    processState();
    nPreState = nState;
}

void testApp::processState(){
    
    //  One way
    //
    if ( (nState == 2) && ( nPreState != 2)){
        
        if (bPlayMode){
        	if (nFrame+1 < nFrameMax){
                nFrame++;
            }else{
            	nFrame=firstFrame;
            }
            
            actual.loadImage(ofToString(nFrame,8,'0')+".jpg");
            	
        } else {
            recordNewFrame();
        }

        actual.update();
        
    }
    
    /*
     
     // To ways
     //
    if ( (nState == 1) && (nPreState != 1)){
        
        // Going Foward
        //
        if (bPlayMode){
            if (nFrame+1 < nFrameMax)
                nFrame++;
                
            	std::cout << "play " << nFrame << std::endl;
            	actual.loadImage(ofToString(nFrame,8,'0')+".jpg");
            	
        } else {
            recordNewFrame();
        }

        actual.update();
        
        return;
        
        
    } else if ( (nState == 2) && ( nPreState != 2)){
        
        //  Crossing over the windows
        //
        //if (!bPlayMode ){
            //actual.saveImage(ofToString(nFrame,8,'0')+".jpg", OF_IMAGE_QUALITY_LOW);
            //bFrameRecorded = true;
        //}
        /// solo va en un sentido
        //if (bPlayMode){
            //if (nFrame+1 < nFrameMax){
                //nFrame++;
            //}else if(nFrame-1 >= 0){
            	//nFrame--;
            //}
            	//std::cout << "play " << nFrame << std::endl;
            	//actual.loadImage(ofToString(nFrame,8,'0')+".jpg");
            	
        //} else {
            //recordNewFrame();
        //}

        //actual.update();
        //return;
        
    } else if ( (nState == 3) && (nPreState != 3)){
        
        // Going Backguards 
        //
        if (bPlayMode){
            if (nFrame-1 >= 0)
                nFrame--;
            
            actual.loadImage(ofToString(nFrame,8,'0')+".jpg");
        } else {
            recordNewFrame();
        }
        
        actual.update();
       
        return;
    }*/
}

void testApp::recordNewFrame(){
    
     
    //  Copy the camera content into the actual image
    //                       
#ifdef USE_GST
    actual.setFromPixels(cam.getPixels(),
                         cam.getWidth(), cam.getHeight(),
                         OF_IMAGE_GRAYSCALE);
#else
    actual.setFromPixels(cam.getPixels(),
                         cam.getWidth(), cam.getHeight(),
                         OF_IMAGE_COLOR);
#endif
    
    
    if (!bPlayMode ){
        actual.saveImage(ofToString(nFrame,8,'0')+".jpg", OF_IMAGE_QUALITY_LOW);
        bFrameRecorded = true;
    }
    
    //  New frames allways go at the end
    //
    nFrame = nFrameMax-1;
    

    //  If its at the head of the film add one more frame to it
    //
    if (nFrame+1 >= nFrameMax )
        nFrameMax++;
    
    //  Go forward;
    //
    if (nFrame+1 < nFrameMax)
        nFrame++;
        
    if(!bNotUsed){
    	bNotUsed = nFrame;
    	bNotUsed = true;
    }
}


//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
    
    int colorValue = 255;
#ifdef TARGET_RASPBERRY_PI
    colorValue = ofMap(analogIn.value, 0, 1024, 0, 255, true);
#endif
    ofSetColor(colorValue);
    
    
    //  Shader not working on RaspberryPi :S
    //
//    shader.begin();
//    shader.setUniform2f("resoultion", (float)width, (float)height);
//    shader.setUniform1f("time", ofGetElapsedTimef());
//    shader.setUniform1f("freq", ofGetFrameRate());
//    actual.draw(0, 0);
//    shader.end();
    
    //  Centered and well scale image
    //
    actual.draw(ofGetWidth()*0.5 - actual.width*0.5, ofGetHeight()*0.5 - actual.height*0.5);
    
    //  Debug
    //
    if (bDebug){
        stringstream info;
        info << "Fps: "   << ofGetFrameRate() << "\n";
        info << "Frame: " << nFrame << "/" << nFrameMax << "\n";
        info << "State: " << nState << "\n";
        info << "Light: " << analogIn.value << ((bPlayMode)?" (Play)":" (Rec)") << "\n";
        ofDrawBitmapStringHighlight(info.str(), 15, 15, ofColor::black, ofColor::white);
        
        if ( bFrameRecorded ){
            ofSetColor(255,0,0);
            ofCircle( ofGetWidth() - 20, ofGetHeight()*0.5 - 20, 10);
            bFrameRecorded = false;
        }
    } else {
        if ( bFrameRecorded ){
            bFrameRecorded = false;
        }
    }
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
    
    if (key == ' '){
        if (!bPlayMode){
            analogIn.value = 20;
        } else {
            analogIn.value = 1024;
        }
    } else if ( key == OF_KEY_RIGHT){
        bNext = true;
        bPrev = false;
    } else if ( key == OF_KEY_LEFT){
        bNext = false;
        bPrev = true;
    } else if ( key == 'd'){
        bDebug = !bDebug;
    } else if ( key == 'f'){
        ofToggleFullscreen();
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	bNext = false;
    bPrev = false;
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
    width = w;
    height = h;
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

