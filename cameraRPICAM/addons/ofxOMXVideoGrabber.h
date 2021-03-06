/*
 *  ofxOMXVideoGrabber.h
 *
 *  Created by jason van cleave on 6/1/13.
 *
 */

#pragma once

#ifdef TARGET_RASPBERRY_PI
#include "ofMain.h"
#include "ofAppEGLWindow.h"

#include <IL/OMX_Core.h>
#include <IL/OMX_Component.h>
#include <IL/OMX_Index.h>
#include <IL/OMX_Image.h>
#include <IL/OMX_Video.h>
#include <IL/OMX_Broadcom.h>

class ofxOMXVideoGrabber  { //: public ofBaseVideoGrabber{
public:
	ofxOMXVideoGrabber();
	~ofxOMXVideoGrabber();
    
	void    initGrabber(int _width=1280, int _height=720);
    void    setDesiredFrameRate(int framerate);
    
    bool    isInitialized();
    
    ofTexture& getTextureReference();
    float	getWidth();
    float	getHeight();
	
    void    update();
    
    void    draw(float x, float y, float w, float h);
    void    draw(float x, float y);
    
	void    close();
    
	void    applyImageFilter(OMX_IMAGEFILTERTYPE imageFilter);
    
    GLuint textureID;
	ofTexture tex;
    
private:
	void    onCameraEventParamOrConfigChanged();
    
    OMX_ERRORTYPE   disableAllPortsForComponent(OMX_HANDLETYPE* m_handle);
    
	static  OMX_ERRORTYPE cameraEventHandlerCallback(OMX_HANDLETYPE hComponent, OMX_PTR pAppData,  OMX_EVENTTYPE eEvent, OMX_U32 nData1, OMX_U32 nData2, OMX_PTR pEventData);
	static  OMX_ERRORTYPE renderEventHandlerCallback(OMX_HANDLETYPE hComponent, OMX_PTR pAppData, OMX_EVENTTYPE eEvent, OMX_U32 nData1, OMX_U32 nData2, OMX_PTR pEventData);
	static  OMX_ERRORTYPE renderEmptyBufferDone	(OMX_IN OMX_HANDLETYPE hComponent, OMX_IN OMX_PTR pAppData, OMX_IN OMX_BUFFERHEADERTYPE* pBuffer);
	static  OMX_ERRORTYPE renderFillBufferDone	(OMX_IN OMX_HANDLETYPE hComponent, OMX_IN OMX_PTR pAppData, OMX_IN OMX_BUFFERHEADERTYPE* pBuffer);
	
	void    generateEGLImage();
    
    OMX_HANDLETYPE  camera;
	OMX_HANDLETYPE  render;
    OMX_BUFFERHEADERTYPE* eglBuffer;
    EGLImageKHR eglImage;
	EGLDisplay display;
	EGLContext context;

    int     width;
	int     height;
    
	int     desiredFramerate;
	
    bool    grabberRunning;
	bool    bInitialized;
};

#endif