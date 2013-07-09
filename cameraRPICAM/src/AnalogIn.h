//
//  AnalogIn.h
//
//  Created by Patricio Gonzalez Vivo on 4/26/13.
//

#pragma once

#include "ofMain.h"

#ifdef TARGET_RASPBERRY_PI
#include "wiringPiSPI.h"
#endif

class AnalogIn: public ofThread {
public:
	AnalogIn();
    
	bool    setup();
	
	int     lastValue;
	int     value;
	int     changeAmount;
	
	bool    doToleranceCheck;
	int     tolerance;
private:
    void    threadedFunction();
	int     readAnalogDigitalConvertor();
    int     input;
    bool    didChange;
	bool    isReady;
};


