#include "testApp.h"
#define STRINGIFY(A) #A

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
	cam.setPixelFormat(OF_PIXELS_MONO);
    cam.initGrabber(camWidth,camHeight,false);
#endif
    cam.update();
    actual.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
    
    //  Load Shader
    //
    shader.load("", "oldFilm.fs");
    loadLUT("Gotham.cube");
    
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
    
    width = ofGetWidth();
    height = ofGetHeight();
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
    if (analogIn.value > 512)
        bPlayMode = true;
    else
        bPlayMode = false;
    
    
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
    
    if ( (nState == 1) && (nPreState == 0) ){
        
        // Going Foward
        //
        if (bPlayMode){
            if (nFrame+1 < nFrameMax)
                nFrame++;
            
            actual.loadImage(ofToString(nFrame)+".jpg");
        } else {
            recordNewFrame();
        }
        
        actual.update();
    } else if ( (nState == 2) && ( nPreState != 2)){
        
        //  Crossing over the windows
        //
        if (!bPlayMode ){
            actual.saveImage(ofToString(nFrame)+".jpg", OF_IMAGE_QUALITY_MEDIUM);
            bFrameRecorded = true;
        }
        
    } else if ( (nState == 3) && ( nPreState == 0)){
        
        // Going Backguards 
        //
        if (bPlayMode){
            if (nFrame-1 >= 0)
                nFrame--;
            
            actual.loadImage(ofToString(nFrame)+".jpg");
        } else {
            recordNewFrame();
        }
        
        actual.update();
    }
}

void testApp::recordNewFrame(){
    
    //  Copy the camera content into the actual image
    //
    int w = cam.getWidth();
    int h = cam.getHeight();
    int nPixels = w*h;
    unsigned char * pixels = actual.getPixels();
    unsigned char * pixelsRGB = cam.getPixels();
    for(int i = 0; i < nPixels; i++){
#ifdef USE_GST
        pixels[i] = pixelsRGB[i];
#else
        pixels[i] = pixelsRGB[i*3]; // take the red channel
#endif
    }
    actual.setFromPixels(pixels, w, h, OF_IMAGE_GRAYSCALE);
    
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
    
    shader.begin();
    shader.setUniformTexture("tex0",actual,0);
    shader.setUniformTexture("lut",lutTex, 1);
    shader.setUniform2f("windows", (float)width, (float)height);
    shader.setUniform1f("contrast", 1.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
    glTexCoord2f(width, 0); glVertex3f(width, 0, 0);
    glTexCoord2f(width, height); glVertex3f(width, height, 0);
    glTexCoord2f(0,height);  glVertex3f(0,height, 0);
    glEnd();
    shader.end();
    
//    actual.draw(ofGetWidth()*0.5 - actual.width*0.5, ofGetHeight()*0.5 - actual.height*0.5);
    
    stringstream info;
    info << "Fps: "   << ofGetFrameRate() << "\n";
    info << "Frame: " << nFrame << "/" << nFrameMax << "\n";
	info << "State: " << nState << "\n";
	info << "Light: " << analogIn.value << ((bPlayMode)?" (Play)":" (Rec)") << "\n";
    ofDrawBitmapStringHighlight(info.str(), 15, 15, ofColor::black, ofColor::white);
    
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
    } else if ( key == OF_KEY_RIGHT){
        bNext = true;
        bPrev = false;
    } else if ( key == OF_KEY_LEFT){
        bNext = false;
        bPrev = true;
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

//--------------------------------------------------------------
void testApp::loadLUT(string _lutFile){
    ofBuffer buffer = ofBufferFromFile( _lutFile );
    
    int mapSize = 32;
    int width = mapSize * mapSize;
    int height = mapSize;
    
    float * pixels = new float[mapSize * mapSize * mapSize * 3];
    
    lutTex.allocate( width, height, GL_RGB32F);
    for(int z=0; z<mapSize ; z++){
        for(int y=0; y<mapSize; y++){
            for(int x=0; x<mapSize; x++){
                string content = buffer.getNextLine();
                int pos = x + y*height + z*width;
                
                vector <string> splitString = ofSplitString(content, " ", true, true);
                
                if (splitString.size() >=3) {
                    pixels[pos*3 + 0] = ofToFloat(splitString[0]);
                    pixels[pos*3 + 1] = ofToFloat(splitString[1]);
                    pixels[pos*3 + 2] = ofToFloat(splitString[2]);
                }
            }
        }
    }
    lutTex.loadData( pixels, width, height, GL_RGB);
}
