#include "ofxLeapTouch.h"

ofxLeapTouch::ofxLeapTouch() {
	touchMode = TOUCH_VIA_FINGERS;
}

ofxLeapTouch::~ofxLeapTouch() {}

void ofxLeapTouch::setup(){
	//ofxGui
	gui.setup("bla","gui",20,40);
	gui.add(minX.setup("min X",-200,-400,100));
	gui.add(maxX.setup("max X",130,-100,400));
	gui.add(minY.setup("min Y",50,0,200));
	gui.add(maxY.setup("max Y",270,200,500));
	gui.add(minZ.setup("min Z",-70,-200,100));
	gui.add(maxZ.setup("max Z",100,0,300));
	gui.add(pressedZ.setup("pressed Z",-30,-100,150));
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
			if(touchMode == TOUCH_VIA_FINGERS || touchMode == TOUCH_VIA_BOTH ){

				for(int j = 0; j < hands[i].fingers().count(); j++){
					ofPoint pt;

					const Finger & finger = hands[i].fingers()[j];
					pt = leap.getofPoint( finger.tipPosition() );

					//save finger tip as screenCoords
					touchlessTouchPoint & fingerTip = fingerTips[finger.id()];
					fingerTip = getScreenCoord(pt);

					//events
					touchlessToTouch(fingerTip,finger.id());

					//store fingers seen this frame
					fingersFound.push_back(finger.id());
				}
			}

			// --------- HANDS ---------- //
			if(touchMode == TOUCH_VIA_HANDS || touchMode == TOUCH_VIA_BOTH){

				//TODO hand id * -1
				ofPoint pt;

				pt = leap.getofPoint( hands[i].palmPosition() );

				//save finger tip as screenCoords
				touchlessTouchPoint & handPos = handPositions[hands[i].id()];
				handPos = getScreenCoord(pt);

				//events
				touchlessToTouch(handPos, hands[i].id());

				//store hand
				handsFound.push_back(hands[i].id());
			}
		}
	}

	if(bMarkFrameAsOld)
		leap.markFrameAsOld();

	return isFrameNew;
}

void ofxLeapTouch::touchlessToTouch(touchlessTouchPoint & touchlessP, int id){
	if(touchlessP.z < pressedZ){
		ofTouchEventArgs touch;
		touch.x=touchlessP.x;
		touch.y=touchlessP.y;
		touch.id=id;

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
			ofTouchEventArgs touch;
			touch.x=touchlessP.x;
			touch.y=touchlessP.y;
			touch.id=id;

			ofNotifyEvent(ofEvents().touchUp, touch, this);
		}
		touchlessP.bPressed = false;
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
	screenPoint.x = ofMap(fingerTip.x,minX,maxX,0,ofGetScreenWidth());
	screenPoint.y = ofMap(fingerTip.y,maxY,minY,0,ofGetScreenHeight());
	screenPoint.z = fingerTip.z;
	return screenPoint;
}

