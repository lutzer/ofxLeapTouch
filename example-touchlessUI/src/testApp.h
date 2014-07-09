#pragma once

#include "ofMain.h"
#include "ofxLeapTouch.h"
#include "ofxTangibleHandle.h"
#include "ofxTangibleBezierHelper.h"

class touchlessUIExample : public ofBaseApp{

	public:
		void setup();
		void setupGui();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void exit();

		ofxLeapTouch leapTouch;

		ofxTangibleXFixedHandle xfixed;
		ofxTangibleHandle handle;
		ofxTangibleYFixedHandle yfixed;

		ofxTangibleBezierHelper r1,r2,r3;
		ofxTangibleBezierHelper h1,h2;
};
