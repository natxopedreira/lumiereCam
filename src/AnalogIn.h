//
//  AnalogIn.h
//
//  Created by Patricio Gonzalez Vivo on 4/26/13.
//  Based on by jason van cleave on 3/9/13 wich is a translation of http://learn.adafruit.com/reading-a-analog-in-and-controlling-audio-volume-with-the-raspberry-pi/connecting-the-cobbler-to-a-mcp3008
//  and http://raspberrypihobbyist.blogspot.com/2012/12/analog-interface.html
//

#ifdef OF_TARGET_LINUXARMV6L

#pragma once

#include "ofMain.h"
#include "wiringPi.h"

class AnalogIn: public ofThread {
public:
	AnalogIn();
    
	bool    setup();
	void    threadedFunction();
	int     readAnalogDigitalConvertor();
    
	int     potentiometerInput;
	
	int     lastPotValue;
	int     potValue;
	int     changeAmount;
	bool    didPotChange;
	
	bool    doToleranceCheck;
	int     tolerance;
	
	bool    isReady;
};

#endif
