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

class GuiGear : public Gear {
public:
    
    GuiGear();
    
    void    update();
    void    draw();
    
    string  type;
    
    bool    bSelected;
    bool    bPrint;
    
private:
    float   prevAngle;
    bool    bUpdating;
    
};

#endif
