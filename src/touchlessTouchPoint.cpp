#include "touchlessTouchPoint.h"

touchlessTouchPoint::touchlessTouchPoint()
		:bPressed(false){
		touchType = TOUCH_TYPE_UNDEFINED;
	}

touchlessTouchPoint::touchlessTouchPoint(const ofPoint & other){
	x = other.x;
	y = other.y;
	z = other.z;
	last = other;
	bPressed = false;
	touchType = TOUCH_TYPE_UNDEFINED;
}

touchlessTouchPoint & touchlessTouchPoint::operator= (const ofPoint & other){
	last = *this;
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

float touchlessTouchPoint::zDiff(){
	return abs(last.z - z);
}

bool touchlessTouchPoint::ignoreDepthMov(float ignoreFactor){
	return ignoreFactor*zDiff() > abs(last.x-x) && ignoreFactor*zDiff() > abs(last.y -y );
}

