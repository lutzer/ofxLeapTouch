#include "ofxLeapTouch.h"

ofEvent<ofTouchEventArgs> ofxLeapTouch::hoverMoved = ofEvent<ofTouchEventArgs>();
ofEvent<ofTouchEventArgs> ofxLeapTouch::subtleMoved = ofEvent<ofTouchEventArgs>();
ofEvent<ofTouchEventArgs> ofxLeapTouch::hoverIn = ofEvent<ofTouchEventArgs>();
ofEvent<ofTouchEventArgs> ofxLeapTouch::subtleIn = ofEvent<ofTouchEventArgs>();

ofxLeapTouch::ofxLeapTouch() {
	touchMode = TOUCH_VIA_FINGERS;
}

ofxLeapTouch::~ofxLeapTouch() {
#ifndef USE_OFX_GUI
	minX = -200;
	maxX = 130;
	minY = 50;
	maxY = 270;
	minZ = -70;
	maxZ = 100;
	pressedFingerZ = -30;
	pressedHandZ = -30;
	hoverFingerZ = 100;
	hoverHandZ = 100;
	zDiffIgnoreFactor = 1;
#endif

}

void ofxLeapTouch::setup(){
#ifdef USE_OFX_GUI
	//ofxGui
	gui.setup("leap touch gui","gui.xml",20,40);
	gui.add(minX.setup("min X",-200,-400,100));
	gui.add(maxX.setup("max X",130,-100,400));
	gui.add(minY.setup("min Y",50,0,200));
	gui.add(maxY.setup("max Y",270,200,500));
	gui.add(minZ.setup("min Z",-70,-200,100));
	gui.add(maxZ.setup("max Z",100,0,300));
	gui.add(pressedFingerZ.setup("pressed Z finger",-30,-100,150));
	gui.add(pressedHandZ.setup("pressed Z hand",-30,-100,150));
	gui.add(hoverFingerZ.setup("hover Z finger",100,-100,250));
	gui.add(hoverHandZ.setup("hover Z hand",100,-100,250));
	gui.add(zDiffIgnoreFactor.setup("zDiff ignore factor",1,0,10));
	gui.loadFromFile("gui.xml");
#endif

	//ofxLeapMotion
	leap.open();
}

bool ofxLeapTouch::update(bool bMarkFrameAsOld){
	fingersFound.clear();
	handsFound.clear();

	bool isFrameNew = leap.isFrameNew();

	vector<Hand> hands = leap.getLeapHands();
	if( isFrameNew && hands.size() ){

		for(int i = 0; i < (int)hands.size(); i++){

			// -------- FINGERS -------- //
			if(touchMode == TOUCH_VIA_FINGERS || touchMode == TOUCH_VIA_BOTH || touchMode == TOUCH_VIA_ONE_FINGER){
				float minZ = 1000.f;
				int bestFingerId = -1;
				for(int j = 0; j < hands[i].fingers().count(); j++){
					ofPoint pt;

					const Finger & finger = hands[i].fingers()[j];
					pt = leap.getofPoint( finger.tipPosition() );

					//save finger tip as screenCoords
					touchlessTouchPoint & fingerTip = fingerTips[finger.id()];
					fingerTip = getScreenCoord(pt);
					fingerTip.touchType = TOUCH_TYPE_FINGER;

					//events
					touchlessToTouch(fingerTip,finger.id());

					//store fingers seen this frame
					if(touchMode != TOUCH_VIA_ONE_FINGER){
						fingersFound.push_back(finger.id());
					}else{
						if(fingerTip.z < minZ){
							minZ = fingerTip.z;
							bestFingerId = finger.id();
						}
					}
				}
				if(touchMode == TOUCH_VIA_ONE_FINGER){
					fingersFound.push_back(bestFingerId);
				}
			}

			// --------- HANDS ---------- //
			if(touchMode == TOUCH_VIA_HANDS || touchMode == TOUCH_VIA_BOTH
					|| touchMode == TOUCH_VIA_CLOSEDHANDS || touchMode == TOUCH_VIA_OPENHANDS){

				//TODO hand id * -1
				ofPoint pt;

				pt = leap.getofPoint( hands[i].palmPosition() );

				//save finger tip as screenCoords
				touchlessTouchPoint & handPos = handPositions[hands[i].id()];
				handPos = getScreenCoord(pt);
				handPos.touchType = TOUCH_TYPE_HAND;

				//events
				touchlessToTouch(handPos, hands[i].id(),hands[i].fingers().count());

				//store hand
				handsFound.push_back(hands[i].id());
			}
		}
	}

	if(bMarkFrameAsOld)
		leap.markFrameAsOld();

	return isFrameNew;
}

void ofxLeapTouch::touchlessToTouch(touchlessTouchPoint & touchlessP, int id, int fingerCount){
	bool validTouch = true;
	if(touchMode == TOUCH_VIA_OPENHANDS){
		validTouch = fingerCount >= 3;
	}else if(touchMode == TOUCH_VIA_CLOSEDHANDS){
		validTouch = fingerCount < 2; //2 because of the thumb, which often gets detected even if the hand is closed //TODO differ based on the finger length
	}

	bool isPressed = false;
	bool isHovered = false;
	if(touchlessP.touchType == TOUCH_TYPE_FINGER){
		isPressed = touchlessP.z < pressedFingerZ;
		isHovered = !isPressed && touchlessP.z < hoverFingerZ;
	}else if(touchlessP.touchType == TOUCH_TYPE_HAND){
		isPressed = touchlessP.z < pressedHandZ;
		isHovered = !isPressed && touchlessP.z < hoverHandZ;
	}


	ofTouchEventArgs touch;
	touch.x=touchlessP.x / ofGetWidth();
	touch.y=touchlessP.y / ofGetHeight();
	touch.id=id;
	touch.pressure = touchlessP.z;

	//PRESSED
	if(isPressed && validTouch){

		if(touchlessP.state != PRESSED){
			//event -> touch down = hover out
			ofNotifyEvent(ofEvents().touchDown, touch, this);
			touchlessP.state = PRESSED;
		}else{
			//event -> touch moved/dragged
			if(!touchlessP.ignoreDepthMov(zDiffIgnoreFactor)){
				ofNotifyEvent(ofEvents().touchMoved, touch, this);
			}
		}
	}else if(validTouch){
		if(touchlessP.state == PRESSED){
			//event -> touch up

			ofNotifyEvent(ofEvents().touchUp, touch, this);
		}

		//HOVER
		if(isHovered){
			if(touchlessP.state != HOVERED){
				//event -> hover in = subtle out
				ofNotifyEvent(ofxLeapTouch::hoverIn,touch,this);
				touchlessP.state = HOVERED;
			}else{
				//event -> touchless/hover moved
				ofNotifyEvent(ofxLeapTouch::hoverMoved,touch,this);
			}
		}
		//SUBTLE INTERACTION
		else{
			if(touchlessP.state != SUBTLE){
				//event -> subtle in
				if(touchlessP.state == NEW){
					cout << "new finger id:" << id << endl;
				}else{
					cout << "back from hover id: " << id << endl;
				}
				ofNotifyEvent(ofxLeapTouch::subtleIn,touch,this);
				touchlessP.state = SUBTLE;
			}else{
				//event -> subtle movement
				ofNotifyEvent(ofxLeapTouch::subtleMoved,touch,this);
			}

		}
	}
}

void ofxLeapTouch::drawFingers(){
	ofPushStyle();
	for(int i = 0; i < (int)fingersFound.size(); i++){
		int id = fingersFound[i];

		touchlessTouchPoint & tip = fingerTips[id];

		ofSetColor(255,255,255);
		float radius = ofMap(tip.z,maxZ,minZ,0,10);
		if(tip.state == PRESSED){
			ofSetColor(200, 50, 30);
			radius *= 2;
		}else if(tip.state == HOVERED){
			ofSetColor(200, 50, 30,150);
		}
		ofCircle(tip.x,tip.y,radius);
	}
	ofPopStyle();
}

void ofxLeapTouch::drawHands(){
	ofPushStyle();
	for(int i = 0; i < (int)handsFound.size(); i++){
		int id = handsFound[i];

		touchlessTouchPoint & pos = handPositions[id];

		ofSetColor(180,180,180);
		float radius = ofMap(pos.z,maxZ,minZ,0,10) * 2;
		if(pos.state == PRESSED){
			ofSetColor(200, 50, 30);
			radius *= 2;
		}else if(pos.state == HOVERED){
			ofSetColor(200, 50, 30,150);
		}
		ofCircle(pos.x,pos.y,radius);
	}
	ofPopStyle();
}

ofPoint ofxLeapTouch::getScreenCoord(ofPoint & fingerTip){
	ofPoint screenPoint;
	screenPoint.x = ofMap(fingerTip.x,minX,maxX,0,ofGetWidth());
	screenPoint.y = ofMap(fingerTip.y,maxY,minY,0,ofGetHeight());
	screenPoint.z = fingerTip.z;
	return screenPoint;
}

