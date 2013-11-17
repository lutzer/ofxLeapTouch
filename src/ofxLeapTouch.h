#pragma once

#include "ofMain.h"
#include "ofxLeapMotion.h"

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

enum leapTouchType{
	TOUCH_TYPE_HAND,
	TOUCH_TYPE_FINGER,
	TOUCH_TYPE_UNDEFINED
};

using namespace std;

struct touchlessTouchPoint : public ofPoint {
	touchlessTouchPoint()
		:bPressed(false){
		touchType = TOUCH_TYPE_UNDEFINED;
	}

	touchlessTouchPoint(const ofPoint & other){
		x = other.x;
		y = other.y;
		z = other.z;
		last = other;
		bPressed = false;
		touchType = TOUCH_TYPE_UNDEFINED;
	}

	touchlessTouchPoint & operator= (const ofPoint & other){
		last = *this;
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	float zDiff(){
		return abs(last.z - z);
	}

	bool ignoreDepthMov(float ignoreFactor){
		return ignoreFactor*zDiff() > abs(last.x-x) && ignoreFactor*zDiff() > abs(last.y -y );
	}

	bool bPressed;
	leapTouchType touchType;
	ofPoint last;
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
