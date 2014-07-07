#include "testApp.h"

bool bDrawGui = false;

//--------------------------------------------------------------
void leapTouchExample::setup(){
//	ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(0);
	ofEnableAlphaBlending();

	// ----- leap
	leapTouch.setup();
	leapTouch.touchMode = TOUCH_VIA_ONE_FINGER;
}

//--------------------------------------------------------------
void leapTouchExample::update(){
	leapTouch.update(true);
}

//--------------------------------------------------------------
void leapTouchExample::draw(){

	leapTouch.drawFingers();
	leapTouch.drawHands();

	if(bDrawGui){
		ofSetColor(255,255,255);
		leapTouch.gui.draw();
	}
}

//--------------------------------------------------------------
void leapTouchExample::keyPressed(int key){
	switch(key){
	case 'g':
		bDrawGui = !bDrawGui;
		break;
	case 'f':
		leapTouch.touchMode = TOUCH_VIA_ONE_FINGER;
		break;
	case 'h':
		leapTouch.touchMode = TOUCH_VIA_HANDS;
		break;
	}
}

//--------------------------------------------------------------
void leapTouchExample::keyReleased(int key){

}

//--------------------------------------------------------------
void leapTouchExample::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void leapTouchExample::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void leapTouchExample::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void leapTouchExample::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void leapTouchExample::windowResized(int w, int h){

}

//--------------------------------------------------------------
void leapTouchExample::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void leapTouchExample::dragEvent(ofDragInfo dragInfo){

}
