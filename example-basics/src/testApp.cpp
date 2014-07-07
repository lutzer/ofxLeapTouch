#include "testApp.h"

bool bDrawGui = false;

//--------------------------------------------------------------
void leapTouchExample::setup(){
//	ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(0);
	ofEnableAlphaBlending();

	// ----- leap
	leapTouch.setup(20,100);
	leapTouch.touchMode = TOUCH_VIA_ONE_FINGER;

	ofAddListener(ofEvents().touchDown,this,&leapTouchExample::touchDown);
	ofAddListener(ofEvents().touchUp,this,&leapTouchExample::touchUp);
	ofAddListener(ofEvents().touchMoved,this,&leapTouchExample::touchMoved);
}

//--------------------------------------------------------------
void leapTouchExample::update(){
	leapTouch.update(true);
}

//--------------------------------------------------------------
void leapTouchExample::draw(){

	ofSetColor(200);
	string isConnectedStr = leapTouch.getLeapMotionRef().isConnected() ? " " : " not ";
	string msg = "ofxLeapTouch - Example App\nLeap is"+isConnectedStr+"connected";
	msg += "\npress f: finger touch";
	msg += "\npress h: hand touch";
	msg += "\npress g: toggle gui";

	ofDrawBitmapString(msg, 20, 20);

	leapTouch.drawFingers();
	leapTouch.drawHands();

	if(bDrawGui){
		ofSetColor(255,255,255);
		leapTouch.gui.draw();
	}
}

void leapTouchExample::touchDown(ofTouchEventArgs & touch){
	cout << " cursor added: " + ofToString(touch.id)+
	" X: "+ofToString(touch.x)+
	" Y: "+ofToString(touch.y)
	<< endl;


}

void leapTouchExample::touchUp(ofTouchEventArgs & touch){
	cout << " cursor removed: " + ofToString(touch.id)+
	 " X: "+ofToString(touch.x)+
	 " Y: "+ofToString(touch.y)
	 << endl;

}

void leapTouchExample::touchMoved(ofTouchEventArgs & touch){
	cout << " cursor updated: " + ofToString(touch.id)+
	 " X: "+ofToString(touch.x)+
	 " Y: "+ofToString(touch.y)
	 << endl;
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

void leapTouchExample::exit(){
	ofRemoveListener(ofEvents().touchDown,this,&leapTouchExample::touchDown);
	ofRemoveListener(ofEvents().touchUp,this,&leapTouchExample::touchUp);
	ofRemoveListener(ofEvents().touchMoved,this,&leapTouchExample::touchMoved);
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
