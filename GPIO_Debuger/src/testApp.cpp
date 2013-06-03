#include "testApp.h"
#define STRINGIFY(A) #A

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
    
#ifdef TARGET_RASPBERRY_PI 
    //  Setup WiringPi
    //
    if(wiringPiSetup() == -1){
        printf("Error on wiringPi setup\n");
    }
    
    pinMode(0,INPUT);
    pinMode(3,INPUT);
#endif

    //  Light Sensor
    //
    analogIn.setup();
    bPlayMode = false;
    bFrameRecorded = false;
    
    nState = nPreState = 0;
    
    nFrame  = 0;
    nFrameMax = 0;
}


//--------------------------------------------------------------
void testApp::update(){
    //  Check Light sensor
    //------------------------------------------------
    //
    if (analogIn.value > 512)
        bPlayMode = true;
    else
        bPlayMode = false;
    
    
    //  Check disk state based on A&B Sensors
    //------------------------------------------------
    
    //  2. trough PINS (just raspbery)
#ifdef TARGET_RASPBERRY_PI
    bA = bB = false;
    
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
    
    if ( (nState == 1) && (nPreState == 0) ){
        
        // Going Foward
        //
        if (bPlayMode){
            if (nFrame+1 < nFrameMax)
                nFrame++;
            
//            actual.loadImage(ofToString(nFrame)+".jpg");
        } else {
            recordNewFrame();
        }
        
//        actual.update();
    } else if ( (nState == 2) && ( nPreState != 2)){
        
        //  Crossing over the windows
        //
        if (!bPlayMode ){
//            actual.saveImage(ofToString(nFrame,8,'0')+".jpg", OF_IMAGE_QUALITY_MEDIUM);
            bFrameRecorded = true;
        }
        
    } else if ( (nState == 3) && ( nPreState == 0)){
        
        // Going Backguards 
        //
        if (bPlayMode){
            if (nFrame-1 >= 0)
                nFrame--;
            
//            actual.loadImage(ofToString(nFrame,8,'0')+".jpg");
        } else {
            recordNewFrame();
        }
        
//        actual.update();
    }
}

void testApp::recordNewFrame(){
        
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
}


//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
    
    int colorValue = 255;
#ifdef TARGET_RASPBERRY_PI
    colorValue = ofMap(analogIn.value, 0, 1024, 0, 255, true);
#endif
    ofSetColor(colorValue);

    if (bA){
        ofSetColor(255,0,0);
        ofCircle(ofGetWidth()*0.25, ofGetHeight()*0.5, 10);
    }
    
    if (bB){
        ofSetColor(255,0,0);
        ofCircle(ofGetWidth()*0.75, ofGetHeight()*0.5, 10);
    }
    
    ofSetColor(255);
    ofDrawBitmapString("A", ofGetWidth()*0.25-5, ofGetHeight()*0.5+10);
    ofDrawBitmapString("B", ofGetWidth()*0.75-5, ofGetHeight()*0.5+10);
    
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
    
    
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
    
    if (key == ' '){
        if (bPlayMode){
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

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

