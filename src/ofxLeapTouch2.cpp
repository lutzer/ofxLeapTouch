#include "ofxLeapTouch2.hpp"

ofxLeapTouch::ofxLeapTouch() : ofxLeapTouch(new ofxLeapMotion()) {
}

ofxLeapTouch::ofxLeapTouch(ofxLeapMotion *leap) {
    this->leap = leap;
    touchMode = TOUCH_VIA_FINGERS;
}

ofxLeapTouch::~ofxLeapTouch() {
	leap->close();
}

void ofxLeapTouch::setup(float guiX, float guiY){
	//ofxGui
	gui.setup("leap touch gui","leap_gui.xml",guiX,guiY);
	gui.add(minX.setup("min X",-200,-400,100));
	gui.add(maxX.setup("max X",130,-100,400));
	gui.add(minY.setup("min Y",50,0,500));
	gui.add(maxY.setup("max Y",270,0,500));
	gui.add(minZ.setup("min Z",-70,-200,100));
	gui.add(maxZ.setup("max Z",200,0,300));
	gui.add(pressedFingerZ.setup("pressed Z finger",-30,-100,150));
	gui.add(pressedHandZ.setup("pressed Z hand",-10,-100,150));
	gui.add(hoverFingerZ.setup("hover Z finger",50,-100,250));
	gui.add(hoverHandZ.setup("hover Z hand",70,-100,250));
	gui.add(minGrabStrength.setup("min grab strength",0.4,0,1));
	gui.add(zDiffIgnoreFactor.setup("zDiff ignore factor",1,0,10));
	gui.loadFromFile("leap_gui.xml");

    leap->open();
}

void ofxLeapTouch::calibrateMin(ofPoint bottomLeft) {
    minX = bottomLeft.x;
    minY = bottomLeft.y;
}
void ofxLeapTouch::calibrateMax(ofPoint topRight) {
    maxX = topRight.x;
    maxY = topRight.y;
}

bool ofxLeapTouch::update(bool bMarkFrameAsOld){
	fingersFound.clear();
	handsFound.clear();

	bool isFrameNew = leap->isFrameNew();

	vector<Hand> hands = leap->getLeapHands();
	if( isFrameNew && hands.size() ){

		for(int i = 0; i < (int)hands.size(); i++){

			// -------- FINGERS -------- //
			if(touchMode == TOUCH_VIA_FINGERS || touchMode == TOUCH_VIA_BOTH || touchMode == TOUCH_VIA_ONE_FINGER){
				float minZ = 1000.f;
				int bestFingerId = -1;
				for(int j = 0; j < hands[i].fingers().count(); j++){
					ofPoint pt;

					const Finger & finger = hands[i].fingers()[j];

					pt = leap->getofPoint( finger.tipPosition() );

					//save finger tip in screen coordinates
					touchlessTouchPoint & fingerTip = fingerTips[finger.id()];
					fingerTip = getScreenCoord(pt);
					fingerTip.leapP = pt;
					fingerTip.touchType = TOUCH_TYPE_FINGER;
					fingerTip.extended = finger.isExtended();

					//store and handle fingers seen this frame
					if (touchMode != TOUCH_VIA_ONE_FINGER){
						//store
						fingersFound.push_back(finger.id());
						//handle events
						touchlessToTouch(fingerTip,finger.id());
					} else {
						if (fingerTip.z < minZ) {
							minZ = fingerTip.z;
							bestFingerId = finger.id();
						}
					}
				}
				if (touchMode == TOUCH_VIA_ONE_FINGER && bestFingerId != -1) {
					//store
					fingersFound.push_back(bestFingerId);
					//handle events
					touchlessToTouch(fingerTips[bestFingerId],bestFingerId);
				}
			}

			// --------- HANDS ---------- //
			if (touchMode == TOUCH_VIA_HANDS || touchMode == TOUCH_VIA_BOTH
					|| touchMode == TOUCH_VIA_CLOSEDHANDS || touchMode == TOUCH_VIA_OPENHANDS){

				//TODO hand id * -1
				ofPoint pt;
				Leap::Hand & hand = hands[i];

				//store hand
				handsFound.push_back(hand.id());
				pt = leap->getofPoint( hand.palmPosition() );

				//save hand position in screen coordinates
				touchlessTouchPoint & handPos = handPositions[hands[i].id()];
				handPos = getScreenCoord(pt);
				handPos.leapP = pt;
				handPos.touchType = TOUCH_TYPE_HAND;

				//events
				touchlessToTouch(handPos, hand.id(), hand.grabStrength());
			}
		}
	}

	if(bMarkFrameAsOld)
		leap->markFrameAsOld();

	return isFrameNew;
}

void ofxLeapTouch::touchlessToTouch(touchlessTouchPoint & touchlessP, int id, float grabStrength){
	//check open versus close hand
	bool isValid = true;
	if(touchMode == TOUCH_VIA_OPENHANDS){
		isValid = grabStrength < minGrabStrength;
	} else if (touchMode == TOUCH_VIA_CLOSEDHANDS){
		isValid = grabStrength > minGrabStrength;
	}

	bool isPressed = false;
	bool isHovered = false;
	if (touchlessP.touchType == TOUCH_TYPE_FINGER){
		if(touchlessP.extended){ //non-extended fingers equals subtle
			isPressed = touchlessP.z < pressedFingerZ;
			isHovered = !isPressed && touchlessP.z < hoverFingerZ;
		}
	} else if (touchlessP.touchType == TOUCH_TYPE_HAND){
		isPressed = isValid && touchlessP.z < pressedHandZ;
		isHovered = !isPressed && touchlessP.z < hoverHandZ;
	}

	ofLeapTouchEventArgs touch;
	touch.x=touchlessP.x;
	touch.y=touchlessP.y;
	touch.id=id;
	touch.pressure = touchlessP.z;
    touch.leapTouchPosition = touchlessP.leapP;
    touch.leapFingerId = id;

	//PRESSED
	if(isPressed){

		if(touchlessP.state != PRESSED){
			//event -> touch down = hover out
			ofNotifyEvent(ofEvents().touchDown, touch, this);
            ofNotifyEvent(this->touchDown,touch,this);
			touchlessP.state = PRESSED;
		}else{
			//event -> touch moved/dragged
			if(!touchlessP.ignoreDepthMov(zDiffIgnoreFactor)){
				ofNotifyEvent(ofEvents().touchMoved, touch, this);
                ofNotifyEvent(this->touchMoved,touch,this);
			}
		}
	}else{
		if(touchlessP.state == PRESSED){
			//event -> touch up
			ofNotifyEvent(ofEvents().touchUp, touch, this);
            ofNotifyEvent(this->touchUp,touch,this);
		}

		//HOVER
		if(isHovered){
			if(touchlessP.state != HOVERED){
				//event -> hover in = subtle out
				ofNotifyEvent(this->hoverIn,touch,this);
				touchlessP.state = HOVERED;
			}else{
				//event -> touchless/hover moved
				ofNotifyEvent(this->hoverMoved,touch,this);
			}
		}
		//SUBTLE INTERACTION
		else{
			if(touchlessP.state != SUBTLE){
				//event -> subtle in
				if(touchlessP.state == NEW){
//					cout << "new finger id:" << id << endl;
				}else{
//					cout << "back from hover id: " << id << endl;
				}
				ofNotifyEvent(this->subtleIn,touch,this);
				touchlessP.state = SUBTLE;
			}else{
				//event -> subtle movement
				ofNotifyEvent(this->subtleMoved,touch,this);
			}

		}
	}
}

void ofxLeapTouch::drawFingers(bool leapCoords){
	ofPushStyle();
    
	for(int i = 0; i < (int)fingersFound.size(); i++){
		int id = fingersFound[i];

		touchlessTouchPoint & tip = fingerTips[id];

		ofSetColor(128,128,128,128);
		float radius = ofMap(tip.z,maxZ,minZ,5,20);
		if(tip.state == PRESSED){
			ofSetColor(200, 50, 30);
			radius *= 1.5;
		}else if(tip.state == HOVERED){
			ofSetColor(200, 50, 30, 128);
		}
		ofCircle(tip.x,tip.y,radius);
		if(leapCoords){
			ofSetColor(228);
			ofDrawBitmapString(tip.toString(),tip.x,tip.y);
		}
	}
    
	ofPopStyle();
}

void ofxLeapTouch::drawHands(bool leapCoords){
	ofPushStyle();
	for(int i = 0; i < (int)handsFound.size(); i++){
		int id = handsFound[i];

		touchlessTouchPoint & pos = handPositions[id];

		ofSetColor(128,128,128,128);
		float radius = ofMap(pos.z,maxZ,minZ,10,40);
		if(pos.state == PRESSED){
			ofSetColor(30, 50, 200);
			radius *= 2;
		}else if(pos.state == HOVERED){
			ofSetColor(30, 50, 200, 128);
		}
		ofCircle(pos.x,pos.y,radius);
		if(leapCoords){
			ofSetColor(228);
			ofDrawBitmapString(pos.toString(),pos.x,pos.y);
		}
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
