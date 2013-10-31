#include "ofxLeapTouch.h"

ofEvent<ofTouchEventArgs> ofxLeapTouch::touchlessMoved = ofEvent<ofTouchEventArgs>();

ofxLeapTouch::ofxLeapTouch() {
	touchMode = TOUCH_VIA_FINGERS;
}

ofxLeapTouch::~ofxLeapTouch() {}

void ofxLeapTouch::setup(){
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
	gui.loadFromFile("gui.xml");

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
	if(touchlessP.touchType == TOUCH_TYPE_FINGER){
		isPressed = touchlessP.z < pressedFingerZ;
	}else if(touchlessP.touchType == TOUCH_TYPE_HAND){
		isPressed = touchlessP.z < pressedHandZ;
	}

	ofTouchEventArgs touch;
	touch.x=touchlessP.x / ofGetWidth();
	touch.y=touchlessP.y / ofGetHeight();
	touch.id=id;

	if(isPressed && validTouch){

		if(!touchlessP.bPressed){
			//event -> touch down
			ofNotifyEvent(ofEvents().touchDown, touch, this);
			touchlessP.bPressed = true;
		}else{
			//event -> touch moved
			ofNotifyEvent(ofEvents().touchMoved, touch, this);
		}
	}else{
		if(touchlessP.bPressed){
			//event -> touch up

			ofNotifyEvent(ofEvents().touchUp, touch, this);
			touchlessP.bPressed = false;
		}else{
			//send touchlessMoved
			ofNotifyEvent(ofxLeapTouch::touchlessMoved,touch,this);
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
		if(tip.bPressed){
			ofSetColor(200, 50, 30);
			radius *= 2;
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
		if(pos.bPressed){
			ofSetColor(50, 50, 220);
			radius *= 2;
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

