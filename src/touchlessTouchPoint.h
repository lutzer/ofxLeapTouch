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

	touchPointState state;
	leapTouchType touchType;
	ofPoint last;
};
