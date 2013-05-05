//
//  GuiGear.h
//  gears
//
//  Created by Patricio Gonzalez Vivo on 4/5/13.
//
//

#ifndef GUIGEAR
#define GUIGEAR

#include "ofMain.h"
#include "Gear.h"
#include "ofxArcText.h"

class GuiGear : public Gear {
public:
    
    GuiGear();
    
    void    loadFont(string _font, float _fontSize, int _dpi = 90);
    
    void    update();
    void    draw();
    
    string  type;
    
    bool    bSelected;
    bool    bPrint;
    
private:
    ofxArcText      font;
    bool            bUpdating;
    
};

#endif
