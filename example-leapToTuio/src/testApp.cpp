#include "testApp.h"

bool bDrawGui = false;
bool bDrawDetails = true;

//--------------------------------------------------------------
void leapToTuioExample::setup(){
//	ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(0);
	ofEnableAlphaBlending();

	// ----- leap
	leapTouch.setup(20,100);
	leapTouch.touchMode = TOUCH_VIA_ONE_FINGER;

	ofAddListener(ofEvents().touchDown,this,&leapToTuioExample::touchDown);
	ofAddListener(ofEvents().touchUp,this,&leapToTuioExample::touchUp);
	ofAddListener(ofEvents().touchMoved,this,&leapToTuioExample::touchMoved);

	myTuioServer.start("127.0.0.1",3333);
	myTuioServer.setVerbose(true);
}

//--------------------------------------------------------------
void leapToTuioExample::update(){
	leapTouch.update(true);
	myTuioServer.run();
}

//--------------------------------------------------------------
void leapToTuioExample::draw(){

	ofSetColor(200);
	string isConnectedStr = leapTouch.getLeapMotionRef().isConnected() ? " " : " not ";
	string msg = "ofxLeapTouch - Example App\nLeap is"+isConnectedStr+"connected";
	msg += "\npress f: finger touch";
	msg += "\npress h: hand touch";
	msg += "\npress g: toggle gui";
	msg += "\npress d: toggle details";

	ofDrawBitmapString(msg, 20, 20);

	leapTouch.drawFingers(bDrawDetails);
	leapTouch.drawHands(bDrawDetails);

	if(bDrawGui){
		ofSetColor(255,255,255);
		leapTouch.gui.draw();
	}
}

void leapToTuioExample::touchDown(ofTouchEventArgs & touch){
	cout << " cursor added: " + ofToString(touch.id)+
	" X: "+ofToString(touch.x)+
	" Y: "+ofToString(touch.y)
	<< endl;

	TuioCursor * cursor = myTuioServer.addCursor(touch.x*ofGetWidth(),touch.y*ofGetHeight());
	if(cursors.find(touch.id) == cursors.end())
		cursors.insert(CursorPair(touch.id,cursor));
	else
		cursors.at(touch.id) = cursor;
}


void leapToTuioExample::touchUp(ofTouchEventArgs & touch){
	cout << " cursor removed: " + ofToString(touch.id)+
	 " X: "+ofToString(touch.x)+
	 " Y: "+ofToString(touch.y)
	 << endl;

	myTuioServer.removeCursor(cursors.at(touch.id));
}

void leapToTuioExample::touchMoved(ofTouchEventArgs & touch){
	cout << " cursor updated: " + ofToString(touch.id)+
	 " X: "+ofToString(touch.x)+
	 " Y: "+ofToString(touch.y)
	 << endl;

	myTuioServer.updateCursor(cursors.at(touch.id),touch.x*ofGetWidth(),touch.y*ofGetHeight());
}

//--------------------------------------------------------------
void leapToTuioExample::keyPressed(int key){
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
void leapToTuioExample::keyReleased(int key){
}

void leapToTuioExample::exit(){
	ofRemoveListener(ofEvents().touchDown,this,&leapToTuioExample::touchDown);
	ofRemoveListener(ofEvents().touchUp,this,&leapToTuioExample::touchUp);
	ofRemoveListener(ofEvents().touchMoved,this,&leapToTuioExample::touchMoved);
}

//--------------------------------------------------------------
void leapToTuioExample::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void leapToTuioExample::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void leapToTuioExample::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void leapToTuioExample::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void leapToTuioExample::windowResized(int w, int h){

}

//--------------------------------------------------------------
void leapToTuioExample::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void leapToTuioExample::dragEvent(ofDragInfo dragInfo){

}
