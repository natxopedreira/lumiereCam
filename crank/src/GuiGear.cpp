//
//  GuiGear.cpp
//  gears
//
//  Created by Patricio Gonzalez Vivo on 4/5/13.
//
//

#include "GuiGear.h"

GuiGear::GuiGear(){
    
    x = ofGetWidth()*0.5;
    y = ofGetHeight()*0.5;
    
    cogs = 30;
    spin = 1;
    
    linked = NULL;
    
        data = new GearData();
        data->speed = .5;
        data->hcog = 5;
        data->wcog = 5;
       
    angle = 0.0;
    radius = cogs * data->wcog / PI;
    
    bSelected = true;
    bPrint = false;
    bUpdating = false;
    
    type = "none";
    loadFont("metric.ttf", 12, 120);
}

void GuiGear::loadFont(string _fontLocation, float _fontSize, int _dpi){
    font.loadFont(_fontLocation, _fontSize, true, true);
    font.setGlobalDpi(_dpi);
    font.setLetterSpacing(1.1);
}


void GuiGear::update(){
    step();
    bUpdating = true;
}

void GuiGear::draw(){
    
    if ( !bPrint ){
        
        if ( bSelected ){
            ofPushStyle();
            ofSetColor(255,50);
            drawGear(true);
            ofPopStyle();
        } else {
            drawGear(false);
        }
        
        ofPushMatrix();
        ofPushStyle();
        ofTranslate(x, y);
    
        ofRotate(ofRadToDeg(angle), 0, 0, 1);
        if ( bSelected ){
            ofSetColor( 255,255 );
        } else {
            ofSetColor( ofGetStyle().color , 200);
        }
        font.drawString( type , 0, 0, radius-7);
        
        ofPopStyle();
        ofPopMatrix();
        
        if ( (getSpeed() > 0.0) && bUpdating ){
            ofPushStyle();
            
            float ra = radius+15;
            float a1 = 90+15;
            float a2 = 90+30;
            float a3 = 90-30;
            float a4 = 90-15;
            
            ofSetLineWidth(0.3);
            ofSetColor( 255 , 100);
            
            ofPolyline arrow;
            arrow.arc(x, y, ra,ra, a1,a2, 60);
            arrow.draw();
            arrow.clear();
            arrow.arc(x, y, ra,ra, a3, a4, 60);
            arrow.draw();
            
            float   arrowAngle = ofDegToRad( (spin >= 0.0)? a2 : a3 );
            ofPoint arrowHead = ofPoint(x,y);
            arrowHead.x += cos( arrowAngle ) * ra;
            arrowHead.y += sin( arrowAngle ) * ra;
            
            ofPushMatrix();
            ofTranslate(arrowHead);
            ofRotate( ofRadToDeg(arrowAngle) + ((spin >= 0.0)? 180 : 0) , 0, 0, 1.0);
            ofFill();
            
            ofBeginShape();
            ofVertex(0, 0);
            ofVertex(3, 6);
            ofVertex(-3, 6);
            ofEndShape();
            
            ofPopMatrix();
            
            font.drawString( ofToString(getSpeed(),2) , x, y, ra+5);
            ofPopStyle();
        }
    } else {
        ofPushStyle();
        ofSetColor(0);
        drawGear(false);
        ofPopStyle();
    }
    
    bUpdating = false;
}