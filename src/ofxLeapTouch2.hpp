#pragma once

#include "ofMain.h"
#include "ofxLeapMotion2.h"
#include "touchlessTouchPoint.hpp"
#include "ofxGui.h"
#include "ofxLeapTouchEventArgs.hpp"

enum leapTouchMode{
	TOUCH_VIA_HANDS,
	TOUCH_VIA_FINGERS,
	TOUCH_VIA_ONE_FINGER,
	TOUCH_VIA_BOTH,
	TOUCH_VIA_OPENHANDS,
	TOUCH_VIA_CLOSEDHANDS
};

using namespace std;

class ofxLeapTouch {
public:
	ofxLeapTouch();
    ofxLeapTouch(ofxLeapMotion *leap);
	virtual ~ofxLeapTouch();

	void setup(float guiX = 20, float guiY = 40);

    
    void onButtonPointMinClicked();
    void onButtonPointMaxClicked();

	/* process leap motion data hand send touch events

	Optional parameter to turn off marking current frame as old after processing,
	so that it is possible to do additional stuff with the sensor.
	Important: Application then has to call markFrameAsOld() by itself! */
	bool update(bool markFrameAsOld = true);

	//"debug" draw
	void drawFingers(bool leapCoords = true);
	void drawHands(bool leapCoords = true);

	ofxLeapMotion * getLeapMotionRef(){ return leap;}

	ofxPanel gui;
	ofxFloatSlider minX, maxX, minY, maxY, minZ, maxZ;
	ofxFloatSlider pressedFingerZ, pressedHandZ, hoverFingerZ, hoverHandZ;
	ofxFloatSlider zDiffIgnoreFactor;
	ofxFloatSlider minGrabStrength;
    ofxButton buttonPointMin, buttonPointMax;

	leapTouchMode touchMode;

    ofEvent<ofLeapTouchEventArgs> touchMoved, touchDown, touchUp;
    ofEvent<ofLeapTouchEventArgs> hoverMoved, hoverIn;	//touch vs. hover
    ofEvent<ofLeapTouchEventArgs> subtleMoved, subtleIn;	//hover vs. subtle

protected:
	ofPoint getScreenCoord(ofPoint & finger);

	//process hand and finger information and possibly send touch events
	void touchlessToTouch(touchlessTouchPoint & p, int id, float grabStrength = 0);
    
    ofPoint lastPosition;

	map <int, ofPolyline> fingerTrails;
	map <int, touchlessTouchPoint> fingerTips;
	map <int, touchlessTouchPoint> handPositions;

	ofxLeapMotion *leap;
	vector <int> fingersFound;
	vector <int> handsFound;
};
