#pragma once

#include "ofMain.h"
#include "ofxLeapMotion.h"
#include "ofxGui.h"

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

	ofxPanel gui;
	ofxFloatSlider minX, maxX, minY, maxY, minZ, maxZ, pressedZ;

protected:

	ofPoint getScreenCoord(ofPoint & finger);

	map <int, ofPolyline> fingerTrails;
	map <int, touchlessTouchPoint> fingerTips;

	ofxLeapMotion leap;
	vector <int> fingersFound;
};
