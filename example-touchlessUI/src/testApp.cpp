#include "testApp.h"

bool bDrawGui = false;
bool bDrawDetails = false;

float x,y;

//--------------------------------------------------------------
void touchlessUIExample::setup(){
//	ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(0);
	ofEnableAlphaBlending();

	// ----- leap
	leapTouch.setup(20,100);
	leapTouch.touchMode = TOUCH_VIA_ONE_FINGER;

	// ----- Touchless-Touch UI
	ofPoint center(ofGetWidth()/3.f,ofGetHeight()/3.f);

	//test handles
	handle.setup(center.x,center.y,300,300);
	yfixed.setup(center.x,100,300,125);
	xfixed.setup(100,center.y,125,300);
	//test listening concept
	xfixed.startListeningTo(handle);
	yfixed.startListeningTo(handle);
	//test mutual listening
	handle.startListeningTo(yfixed);
	handle.startListeningTo(xfixed);

	// test rotation
	float radius = 200;
	x = ofGetWidth() * 0.7f;
	y = ofGetHeight() * 0.7f;
	r1.setup(x+radius/2.f,y,radius/3.f,radius/3.f,x,y);
	r2.setup(x-radius/2.f,y,radius/2.f,radius/2.f,x,y);
	r3.setup(x,y+radius/2.f,radius/4.f,radius/4.f,x,y);
	r1.drawType = r2.drawType = r3.drawType = TANGIBLE_DRAW_AS_CIRCLE;

	r1.setKeepSameDistance(true);
	r2.setKeepSameDistance(true);
	r3.setKeepSameDistance(true);

	r2.setColor(ofColor(0,255,100,180));
	r2.setHoverColor(ofColor(0,255,100,255));

	r1.startListeningTo(r2,TANGIBLE_ROTATE);
	r2.startListeningTo(r1,TANGIBLE_ROTATE);
	r1.startListeningTo(r3,TANGIBLE_ROTATE);
	r3.startListeningTo(r1,TANGIBLE_ROTATE);



}

//--------------------------------------------------------------
void touchlessUIExample::update(){
	leapTouch.update(true);
}

//--------------------------------------------------------------
void touchlessUIExample::draw(){

	// --- Status message
	ofSetColor(200);
	string isConnectedStr = leapTouch.getLeapMotionRef().isConnected() ? " " : " not ";
	string msg = "ofxLeapTouch - Example App\nLeap is"+isConnectedStr+"connected";
	msg += "\npress f: finger touch";
	msg += "\npress h: hand touch";
	msg += "\npress g: toggle gui";
	msg += "\npress d: toggle details";

	ofDrawBitmapString(msg, 20, 20);

	// --- Controls
	handle.draw();
	xfixed.draw();
	yfixed.draw();
	ofSetColor(255,255,255);
	ofEllipse(x,y,5,5);
	r1.draw();
	r2.draw();
	r3.draw();

	// --- Touchless-Touch
	leapTouch.drawFingers(bDrawDetails);
	leapTouch.drawHands(bDrawDetails);

	// --- GUI
	if(bDrawGui){
		ofSetColor(255,255,255);
		leapTouch.gui.draw();
	}
}

//--------------------------------------------------------------
void touchlessUIExample::keyPressed(int key){
	switch(key){
	case 'g':
		bDrawGui = !bDrawGui;
		break;
	case 'd':
		bDrawDetails = !bDrawDetails;
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
void touchlessUIExample::keyReleased(int key){
}

void touchlessUIExample::exit(){
}

//--------------------------------------------------------------
void touchlessUIExample::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void touchlessUIExample::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void touchlessUIExample::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void touchlessUIExample::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void touchlessUIExample::windowResized(int w, int h){

}

//--------------------------------------------------------------
void touchlessUIExample::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void touchlessUIExample::dragEvent(ofDragInfo dragInfo){

}
