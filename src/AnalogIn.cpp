//
//  AnalogIn.cpp
//  lumiereCam
//
//  Created by Patricio Gonzalez Vivo on 4/26/13.
//
//

#ifdef TARGET_RASPBERRY_PI

#include "AnalogIn.h"

AnalogIn::AnalogIn(){
    
	isReady         = false;
    didPotChange    = false;
	changeAmount    = 0;
	lastPotValue    = 0;
	potValue        = 0;
    
    //10k trim pot connected to adc #0
    potentiometerInput = 0;
}


bool AnalogIn::setup(){
    
    isReady     = false;
	int status  = wiringPiSPISetup(0, 1000000);
    
	if (status != -1){
		ofLogVerbose() << "wiringPiSetup PASS";
		isReady = true;
	} else {
		ofLogError() << "wiringPiSetup FAIL status: " << status;
	}
    
	if (isReady) {
		startThread(false, true);
	}
    
	return isReady;
}


void AnalogIn::threadedFunction(){
	while ( isThreadRunning() ){
		didPotChange = false;
		
		potValue = readAnalogDigitalConvertor();
		changeAmount = abs(potValue - lastPotValue);
        
		if(changeAmount!=0) {
            didPotChange = true;
        }
        
		lastPotValue = potValue;
        sleep(10);
	}
}

int  AnalogIn::readAnalogDigitalConvertor() {
	uint8_t buffer[3];
    
	buffer[0] = 1;
	buffer[1] = (8+potentiometerInput)<<4;
	buffer[2] = 0;
    
	wiringPiSPIDataRW(0, buffer, 3);
    
	return ((buffer[1]&3) << 8) + buffer[2];
}

#endif