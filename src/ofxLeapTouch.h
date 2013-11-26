#pragma once

#include "ofMain.h"
#include "ofxLeapMotion.h"
#include "touchlessTouchPoint.h"

#define USE_OFX_GUI

#ifdef USE_OFX_GUI
#include "ofxGui.h"
#endif

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
	virtual ~ofxLeapTouch();

	void setup();
	bool update(bool markFrameAsOld = true);
	void drawFingers();
	void drawHands();

	ofxLeapMotion & getLeapMotionObj(){return leap;}

#ifdef USE_OFX_GUI
	ofxPanel gui;
	ofxFloatSlider minX, maxX, minY, maxY, minZ, maxZ;
	ofxFloatSlider pressedFingerZ, pressedHandZ;
	ofxFloatSlider zDiffIgnoreFactor;
#else
	float minX, maxX, minY, maxY, minZ, maxZ;
	float pressedFingerZ, pressedHandZ;
	float zDiffIgnoreFactor;
#endif


	leapTouchMode touchMode;

	static ofEvent<ofTouchEventArgs> touchlessMoved;	//for move vs. dragged (hover ...)

protected:
	ofPoint getScreenCoord(ofPoint & finger);
	void touchlessToTouch(touchlessTouchPoint & p, int id, int fingerCount = 0);

	map <int, ofPolyline> fingerTrails;
	map <int, touchlessTouchPoint> fingerTips;
	map <int, touchlessTouchPoint> handPositions;

	ofxLeapMotion leap;
	vector <int> fingersFound;
	vector <int> handsFound;
};
