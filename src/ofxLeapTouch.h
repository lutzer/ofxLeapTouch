#pragma once

#include "ofMain.h"
#include "ofxLeapMotion.h"
#include "ofxGui.h"

enum leapTouchMode{
	TOUCH_VIA_HANDS,
	TOUCH_VIA_FINGERS,
	TOUCH_VIA_ONE_FINGER,
	TOUCH_VIA_BOTH,
	TOUCH_VIA_OPENHANDS,
	TOUCH_VIA_CLOSEDHANDS
};

enum leapTouchType{
	TOUCH_TYPE_HAND,
	TOUCH_TYPE_FINGER,
	TOUCH_TYPE_UNDEFINED
};

struct touchlessTouchPoint : public ofPoint {
	touchlessTouchPoint()
		:bPressed(false){
		touchType = TOUCH_TYPE_UNDEFINED;
	}

	touchlessTouchPoint(const ofPoint & other){
		x = other.x;
		y = other.y;
		z = other.z;
		bPressed = false;
		touchType = TOUCH_TYPE_UNDEFINED;
	}

	touchlessTouchPoint & operator= (const ofPoint & other){
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	bool bPressed;
	leapTouchType touchType;
};

class ofxLeapTouch {
public:
	ofxLeapTouch();
	virtual ~ofxLeapTouch();

	void setup();
	bool update(bool markFrameAsOld = true);
	void drawFingers();
	void drawHands();

	ofxLeapMotion & getLeapMotionObj(){return leap;}

	ofxPanel gui;
	ofxFloatSlider minX, maxX, minY, maxY, minZ, maxZ, pressedFingerZ, pressedHandZ;

	leapTouchMode touchMode;

	static ofEvent<ofTouchEventArgs> touchlessMoved;

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
