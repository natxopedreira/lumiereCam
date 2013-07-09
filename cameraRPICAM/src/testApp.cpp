#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    
    //  CAMERA
    //
    int camWidth = 640;
	int camHeight = 480;
    int camFps = 30;
#ifdef TARGET_RASPBERRY_PI
    camWidth = 640*2;
    camHeight = 360*2;
	imageFilters.push_back(OMX_ImageFilterNone);
	imageFilters.push_back(OMX_ImageFilterNoise);
	imageFilters.push_back(OMX_ImageFilterEmboss);
	imageFilters.push_back(OMX_ImageFilterNegative);
	imageFilters.push_back(OMX_ImageFilterSketch);
	imageFilters.push_back(OMX_ImageFilterOilPaint);
	imageFilters.push_back(OMX_ImageFilterHatch);
	imageFilters.push_back(OMX_ImageFilterGpen);
	imageFilters.push_back(OMX_ImageFilterAntialias);
	imageFilters.push_back(OMX_ImageFilterDeRing);
	imageFilters.push_back(OMX_ImageFilterSolarize);
	imageFilters.push_back(OMX_ImageFilterWatercolor);
	imageFilters.push_back(OMX_ImageFilterPastel);
	imageFilters.push_back(OMX_ImageFilterSharpen);
	imageFilters.push_back(OMX_ImageFilterFilm);
	imageFilters.push_back(OMX_ImageFilterBlur);
	imageFilters.push_back(OMX_ImageFilterSaturation);
	imageFilters.push_back(OMX_ImageFilterDeInterlaceLineDouble);
	imageFilters.push_back(OMX_ImageFilterDeInterlaceAdvanced);
	imageFilters.push_back(OMX_ImageFilterColourSwap);
	imageFilters.push_back(OMX_ImageFilterWashedOut);
	imageFilters.push_back(OMX_ImageFilterColourPoint);
	imageFilters.push_back(OMX_ImageFilterPosterise);
	imageFilters.push_back(OMX_ImageFilterColourBalance);
	imageFilters.push_back(OMX_ImageFilterCartoon);
    imageFiltersCounter=0;
#endif
    cam.setDesiredFrameRate(camFps);
    cam.initGrabber(camWidth,camHeight);
    cam.update();
    
    
    //  SHADERS
    //
#ifdef TARGET_RASPBERRY_PI
    shader.load("","oldFilmGL2.fs");
//    shader.load("PostProcessing.vert", "PostProcessing.frag");
#else
    shader.load("", "oldFilm.fs");
#endif
    doShader = false;
    
    
    //  INPUT/OUTPUT
    //
#ifdef TARGET_RASPBERRY_PI
    //  Setup WiringPi
    //
//    if(wiringPiSetup() == -1){
//        printf("Error on wiringPi setup\n");
//    }
//    
//    pinMode(0,INPUT);
//    pinMode(3,INPUT);
#endif
    
}

//--------------------------------------------------------------
void testApp::update(){
	if (!cam.isInitialized()) {
		return;
	} else {
        cam.update();
    }
}


//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
	if (!cam.isInitialized()) {
		return;
	}
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()*0.5 - cam.getWidth()*0.5,
                ofGetHeight()*0.5 - cam.getHeight()*0.5);
    
	if (doShader) {
		shader.begin();
		shader.setUniformTexture("tex0", cam.getTextureReference(), 0); //tex, cam.textureID);
		shader.setUniform1f("time", ofGetElapsedTimef());
        shader.setUniform1f("freq", 24.0);
		shader.setUniform2f("resolution", cam.getWidth(), cam.getHeight());
		ofRect(0, 0, cam.getWidth(), cam.getHeight());
		shader.end();
	} else {
        ofFloatPixels pixels;
        cam.getTextureReference().readToPixels(pixels);
        
		cam.draw(0,0);
	}

	ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 100, 100, ofColor::black, ofColor::yellow);
    
    ofPopMatrix();
}

void testApp::exit(){
    cam.close();
    
#ifdef TARGET_RASPBERRY_PI
	bcm_host_deinit();
#endif
}
//--------------------------------------------------------------
void testApp::keyPressed(int key){

	if (key == 's'){
		doShader = !doShader;
	}

#ifdef TARGET_RASPBERRY_PI
	if (key == 'e'){
		cam.applyImageFilter( imageFilters[ ofRandom( imageFilters.size() ) ] );
	}
#endif

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

