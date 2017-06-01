#pragma once

#include "ofPoint.h"

enum leapTouchType{
	TOUCH_TYPE_HAND,
	TOUCH_TYPE_FINGER,
	TOUCH_TYPE_UNDEFINED
};

enum touchPointState{
	PRESSED,
	HOVERED,
	SUBTLE,
	NEW
};

struct touchlessTouchPoint : public ofPoint {
	touchlessTouchPoint();
	touchlessTouchPoint(const ofPoint & other);

	touchlessTouchPoint & operator= (const ofPoint & other);

	float zDiff();

	bool ignoreDepthMov(float ignoreFactor);

	string & toString();

	touchPointState state;
	leapTouchType touchType;
	ofPoint last;
	ofPoint leapP; //leap coordinates
	bool extended; //non-extended fingers equals subtle
private:
	string debugMsg;
};
