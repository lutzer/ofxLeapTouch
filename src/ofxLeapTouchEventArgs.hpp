//
//  ofxLeapTouchEventArgs.hpp
//  1705_Ultrahaptics_Touchpad
//
//  Created by Lutz Reiter on 01.06.17.
//
//

#ifndef ofxLeapTouchEventArgs_hpp
#define ofxLeapTouchEventArgs_hpp

#include <ofMain.h>
#include "ofxLeapMotion2.h"

class ofLeapTouchEventArgs : public ofTouchEventArgs {
    
    public:
    
        ofVec3f leapTouchPosition;
        float touchDistance;
        int leapFingerId = -1;
    
};

#endif /* ofxLeapTouchEventArgs_hpp */
