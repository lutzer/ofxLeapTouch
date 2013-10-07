#include "ofxLeapTouch.h"

ofxLeapTouch::ofxLeapTouch() {}

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

	bool isFrameNew = leap.isFrameNew();

	vector<Hand> hands = leap.getLeapHands();
	if( isFrameNew && hands.size() ){

		for(int i = 0; i < (int)hands.size(); i++){
			for(int j = 0; j < hands[i].fingers().count(); j++){
				ofPoint pt;

				const Finger & finger = hands[i].fingers()[j];
				pt = leap.getofPoint( finger.tipPosition() );

				//save finger tip as screenCoords
				touchlessTouchPoint & fingerTip = fingerTips[finger.id()];
				fingerTip = getScreenCoord(pt);

				if(fingerTip.z < pressedZ){
					ofTouchEventArgs touch;
					touch.x=fingerTip.x;
					touch.y=fingerTip.y;
					touch.id=finger.id();

					if(!fingerTip.bPressed){
						//event -> touch down
						ofNotifyEvent(ofEvents().touchDown, touch, this);
						fingerTip.bPressed = true;
					}else{
						//event -> touch moved
						ofNotifyEvent(ofEvents().touchMoved, touch, this);
					}
				}else{
					if(fingerTip.bPressed){
						//event -> touch up
						ofTouchEventArgs touch;
						touch.x=fingerTip.x;
						touch.y=fingerTip.y;
						touch.id=finger.id();

						ofNotifyEvent(ofEvents().touchUp, touch, this);
					}
					fingerTip.bPressed = false;
				}

				//store fingers seen this frame
				fingersFound.push_back(finger.id());
			}
		}
	}

	if(bMarkFrameAsOld)
		leap.markFrameAsOld();

	return isFrameNew;
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

ofPoint ofxLeapTouch::getScreenCoord(ofPoint & fingerTip){
	ofPoint screenPoint;
	screenPoint.x = ofMap(fingerTip.x,minX,maxX,0,ofGetScreenWidth());
	screenPoint.y = ofMap(fingerTip.y,maxY,minY,0,ofGetScreenHeight());
	screenPoint.z = fingerTip.z;
	return screenPoint;
}

