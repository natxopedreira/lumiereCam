#pragma once

#include "ofMain.h"
#include "GuiGear.h"

//  Lenguaje: none/med/good
//  Education: no degree/HS/BA
//  Network: 0-10/10-100/100->
//  Visa: none/F1-J1-H1/Greencard

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
	
    bool        setPosition(GuiGear *p);
    GuiGear*    nearest(ofPoint *p);
    float       maxRadiusBetween(GuiGear *_g1, GuiGear *_g2);
    void        drawGrid(ofRectangle _rect, float _res);
    
	vector<GuiGear*>   gears;
    GuiGear*           picked;
    
    float           RMIN, RMAX;
    float           tweening;
    
    int             hoverGear;
    
    bool            bValid;
    bool            bPrintToPdf;
};
