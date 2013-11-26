#pragma once

#include "ofPoint.h"

enum leapTouchType{
	TOUCH_TYPE_HAND,
	TOUCH_TYPE_FINGER,
	TOUCH_TYPE_UNDEFINED
};

struct touchlessTouchPoint : public ofPoint {
	touchlessTouchPoint();
	touchlessTouchPoint(const ofPoint & other);

	touchlessTouchPoint & operator= (const ofPoint & other);

	float zDiff();

	bool ignoreDepthMov(float ignoreFactor);

	bool bPressed;
	leapTouchType touchType;
	ofPoint last;
};
