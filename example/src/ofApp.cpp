#include "ofApp.h"

bool bDrawGui = true;
bool bDrawDetails = true;

//--------------------------------------------------------------
void ofApp::setup(){
    //	ofSetLogLevel(OF_LOG_VERBOSE);
    ofBackground(0);
    ofEnableAlphaBlending();
    
    // ----- leap
    leapTouch.setup(20,100);
    leapTouch.touchMode = TOUCH_VIA_ONE_FINGER;
    
    // ----- forward touch events - just for debug messages
    ofAddListener(ofEvents().touchDown,this,&ofApp::touchDown);
    ofAddListener(ofEvents().touchUp,this,&ofApp::touchUp);
    ofAddListener(ofEvents().touchMoved,this,&ofApp::touchMoved);
}

//--------------------------------------------------------------
void ofApp::update(){
    leapTouch.update(true);
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(200);
    string isConnectedStr = leapTouch.getLeapMotionRef()->isConnected() ? " " : " not ";
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

void ofApp::touchDown(ofTouchEventArgs & touch){
    cout << " cursor added: " + ofToString(touch.id)+
    " X: "+ofToString(touch.x)+
    " Y: "+ofToString(touch.y)
    << endl;
    
    
}

void ofApp::touchUp(ofTouchEventArgs & touch){
    cout << " cursor removed: " + ofToString(touch.id)+
    " X: "+ofToString(touch.x)+
    " Y: "+ofToString(touch.y)
    << endl;
    
}

void ofApp::touchMoved(ofTouchEventArgs & touch){
    cout << " cursor updated: " + ofToString(touch.id)+
    " X: "+ofToString(touch.x)+
    " Y: "+ofToString(touch.y)
    << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
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
void ofApp::keyReleased(int key){
}

void ofApp::exit(){
    ofRemoveListener(ofEvents().touchDown,this,&ofApp::touchDown);
    ofRemoveListener(ofEvents().touchUp,this,&ofApp::touchUp);
    ofRemoveListener(ofEvents().touchMoved,this,&ofApp::touchMoved);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
