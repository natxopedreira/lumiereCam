#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);

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
    
	doShader = false;
	shader.load("PostProcessing");
	
    omxVideoGrabber.setup(640, 480, 60);
}

//--------------------------------------------------------------
void testApp::update(){
	if (!omxVideoGrabber.isReady) {
		return;
	}
}


//--------------------------------------------------------------
void testApp::draw(){

	if (!omxVideoGrabber.isReady) {
		return;
	}
    
	if (doShader) {
        
		shader.begin();
		shader.setUniformTexture("tex0", omxVideoGrabber.tex, omxVideoGrabber.textureID);
		shader.setUniform1f("time", ofGetElapsedTimef());
		shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
		ofRect(0, 0, ofGetWidth(), ofGetHeight());
		shader.end();
        
	} else {
		omxVideoGrabber.draw();
	}

	ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 100, 100, ofColor::black, ofColor::yellow);
}

void testApp::exit(){
	if (!omxVideoGrabber.isClosed) {
		omxVideoGrabber.close();
	}
    
	bcm_host_deinit();
}
//--------------------------------------------------------------
void testApp::keyPressed  (int key){

	if (key == 's'){
		doShader = !doShader;
	}

	if (key == 'e'){
		omxVideoGrabber.applyImageFilter( imageFilters[ ofRandom( imageFilters.size() ) ] );
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

