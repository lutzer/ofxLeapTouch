#pragma once

#include "ofMain.h"
#include "ofxLeapMotion.h"
#include "ofxGui.h"

enum leapTouchMode{
	TOUCH_VIA_HANDS,
	TOUCH_VIA_FINGERS,
	TOUCH_VIA_BOTH
};

struct touchlessTouchPoint : public ofPoint {
	touchlessTouchPoint()
		:bPressed(false){
	}

	touchlessTouchPoint(const ofPoint & other){
		x = other.x;
		y = other.y;
		z = other.z;
		bPressed = false;
	}

	touchlessTouchPoint & operator= (const ofPoint & other){
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	bool bPressed;
};

class ofxLeapTouch {
public:
	ofxLeapTouch();
	virtual ~ofxLeapTouch();

	void setup();
	bool update(bool markFrameAsOld = true);
	void drawFingers();
	void drawHands();

	ofxPanel gui;
	ofxFloatSlider minX, maxX, minY, maxY, minZ, maxZ, pressedZ;

	leapTouchMode touchMode;

protected:
	ofPoint getScreenCoord(ofPoint & finger);
	void touchlessToTouch(touchlessTouchPoint & p, int id);

	map <int, ofPolyline> fingerTrails;
	map <int, touchlessTouchPoint> fingerTips;
	map <int, touchlessTouchPoint> handPositions;

	ofxLeapMotion leap;
	vector <int> fingersFound;
	vector <int> handsFound;
};
