#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    RMIN = 30;
    RMAX = 200;
    
    picked = new GuiGear();
    picked->set(ofGetWidth()*0.07, ofGetHeight()*0.89);
    picked->setRadius(50);
    picked->update();
    
    gears.push_back(picked);
    picked = new GuiGear();
    
    hoverGear = -1;
    
    bValid = false;
    bPrintToPdf = false;
    
    //ofToggleFullscreen();
}

//--------------------------------------------------------------
void testApp::update(){
    
    picked->set(mouseX,mouseY);
    bValid = setPosition( picked );
    
    hoverGear = -1;
    ofPoint mouse = ofPoint(mouseX,mouseY);
    for (int i = 0; i < gears.size(); i++) {
        if ( gears[i]->inside(mouse) ){
            hoverGear = i;
            break;
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){

    if (bPrintToPdf){
        ofBackground(255);
        ofBeginSaveScreenAsPDF("screenshot-"+ofGetTimestampString()+".pdf", false);
    } else {
        ofBackground(11,94,148);
        ofBackgroundGradient(ofColor(11,94,148), ofColor(0,45,128,100));
        float margin = 10;
        ofRectangle grid = ofRectangle(margin,margin,ofGetWidth()-margin*2.0,ofGetHeight()-margin*2.0);
        
        ofPushStyle();
        
        ofSetColor(200,20);
        ofSetLineWidth(2);
        ofNoFill();
        ofRect(grid);
        ofSetLineWidth(0.3);
        drawGrid(grid, 10);
        
        ofPopStyle();
    }
    
    //  Potentialy New Gear
    //
    if( bValid && !bPrintToPdf) {
        ofSetColor(255,0,0);
        picked->update();
        picked->draw();
    }
    
    //  Draw Gears
    //
    for (int i = 0; i < gears.size(); i++) {
    
        gears[i]->bPrint = bPrintToPdf;
        gears[i]->bSelected = (hoverGear == i);
        
        ofSetColor(255);
        gears[i]->update();
        gears[i]->draw();

    }
    
    if (bPrintToPdf){
        ofEndSaveScreenAsPDF();
        bPrintToPdf = false;
    }
}

//--------------------------------------------------------------
bool testApp::setPosition(GuiGear *g1) {
    
    // We try to locate the gear as close as possible to (x, y)
    // while satisfying distance constraints towards the other gears.
    // Note: this process is implicitly iterative.
    //
//    GuiGear *g1 = p;
    
    // find the nearest gear
    //
    GuiGear *g2 = nearest(g1);
    
    // Primum Movens
    //
    if(g2 == NULL) {
        // and resize it softly
        //
        g1->setRadius( ofLerp(picked->getRadius(),RMIN, 0.1) );
        g1->linked = NULL;
        return true;
    }
    
    // calculate radius needed to touch g2 (using target coordinates)
    ofPoint diff = *g1 - *g2;
    float rtouch = diff.length() - g2->getRadius();
    
    // calculate maximum radius possible without touching any other gears.
    // We are using the actual gear coordinates so the center of the gear
    // can converge over the course of several iterations.
    //
    float rmax = maxRadiusBetween(g1, g2);
    
    // the new radius
    //
    float rnew = min(rtouch, min( rmax, RMAX));
    
    // is the new radius big enough for a new gear?
    //
    if (rnew >= RMIN) {
        g1->setRadius(rnew);
        
        // distance between the gear centers
        //
        float d = rnew + g2->getRadius();
        
        // angle between gear-axis and the x-axis
        //
        float ang = atan2(diff.y, diff.x);
        
        // align the angular rotation
        //
        g1->angle = PI + ((ang - g2->angle) * g2->getRadius() + ang * g1->getRadius()) / g1->getRadius();
        
        // align direction of rotation
        //
        g1->spin = -g2->spin;
        
        // move the gear center to its new location
        //
        g1->x = g2->x + cos(ang) * d;
        g1->y = g2->y + sin(ang) * d;
        
        g1->linked = g2;
        
        return true;
    } else {
        return false;
    }
}

float testApp::maxRadiusBetween(GuiGear *_g1, GuiGear *_g2){
    float rmax = 1000000.0;
    for (int i = 0; i < gears.size(); i++) {
        if (gears[i] == _g1 || gears[i] == _g2)
            continue;
        
        float d = _g1->distance( ofPoint(_g2->x,_g2->y) ) - _g2->getRadius() - _g1->getData()->hcog;
        rmax = min(rmax, d);
    }
    return rmax;
}

// find the gear that is closest to the point (x, y)
// or return null if there's no gear in the vicinity
GuiGear* testApp::nearest(ofPoint *p) {
    float dmax = RMAX;
    GuiGear *result = NULL;
    for (int i = 0; i < gears.size(); i++){
        if (gears[i] == picked)continue;
        
        float dist = gears[i]->distance( ofPoint(*p) ) - gears[i]->getRadius();
        if (dist < dmax) {
            result = gears[i];
            dmax = dist;
        }
    }
    return result;
}

void testApp::drawGrid(ofRectangle _rect, float _res){
    for (float x = 0 ; x < _rect.width; x += _res ){
        ofLine(_rect.x + x, _rect.y, _rect.x + x, _rect.y + _rect.height);
    }
    
    for (float y = 0 ; y < _rect.height; y += _res ){
        ofLine(_rect.x , _rect.y + y, _rect.x + _rect.width, _rect.y + y);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if (hoverGear >= 0){
        if (key == OF_KEY_RIGHT){
            gears[hoverGear]->getData()->speed += 0.1;
        } else if (key == OF_KEY_LEFT){
            gears[hoverGear]->getData()->speed -= 0.1;
        }
    }
    
    if (key == ' '){
        bPrintToPdf = true;
    } else if ( key == 'f'){
        ofToggleFullscreen();
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    //ofPoint mouse = ofPoint(x, y);
    //bValid = setPosition( &mouse );
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if (bValid){
        gears.push_back(picked);
        picked = new GuiGear();
    }
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