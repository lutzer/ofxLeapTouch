ofxLeapTouch
================

ofxLeapTouch is an [openFrameworks](https://github.com/openframeworks/openFrameworks) addon to use the [Leap Motion](https://leapmotion.com) as a Touchless-Touch controller. After setting up a mid-air "touch" sensitive area near your screen, ofxLeapTouch is interpreting interactions in this field like an touch pad and sends basic openFrameworks touch events accordingly. Because of using these [core events](http://openframeworks.cc/documentation/events/ofCoreEvents.html) combining it with touch reactive user interfaces should be straight forward. 

Touchless-Touch is NOT the next step/level for interacting with systems (mainly because of the missing haptical feedback) but it has its advantages (like hover/subtle states) and ofxLeapTouch helps exploring ideas in this field more quickly.


Features 
--------

* Setup for leap-to-screen mapping (touchless touch area)
* Emit `ofTouchEvents` according to finger/hand interactions
* Works with fingers and/or hands (check `enum leapTouchMode`)
* Addional events to work with hover/subtle states 
* Distant to screen (z-Axis) gets send as touch pressure

Examples
--------

### example-basics

Small example with debug draw of fingers (red) or hands (blue). Depending on the leap Z position each finger/hand has the state **subtle** (grey), **hovering** (transparent) or **pressed** (non-transparent).

![example-basics](https://raw.githubusercontent.com/ixds/ofxLeapTouch/master/example-basics.png)

### example-leapToTuio

Using ofxLeapTouch in combination with [ofxTuioWrapper](http://github.com/fx-lange/ofxTuioWrapper/) to send Touchless-Touch interactions as `TuioCursors`. For testing you can use the [tuioExample](https://github.com/fx-lange/ofxTuioWrapper/tree/master/tuioExample) as a client as shown below. Only pressed fingers / hands are forwarded.

![example-leapToTuio screenshot](https://raw.githubusercontent.com/ixds/ofxLeapTouch/master/example-leapToTuio.png)

### example-touchlessTouch

Controlling [ofxTangibleUI](http://github.com/fx-lange/ofxTangibleUI/) basic handles via Touchless-Touch. `ofxTangibleHandle` is listening to touch events by default - no direct connection needed.

![example-touchlessTouch screenshot](https://raw.githubusercontent.com/ixds/ofxLeapTouch/master/example-touchlessTouch.png)

Setup
------

* ofxLeapTouch is builded upon top of [ofxLeapMotion](http://github.com/ofTheo/ofxLeapMotion/), so please follow the install instructions there and try the basic example before you continue.
* To get a copy of the repository you can download the source from [http://github.com/ixds/ofxLeapTouch](http://github.com/ixds/ofxLeapTouch) or, alternatively, you can use git clone: 

  `git clone git://github.com/ixds/ofxLeapTouch.git`

* The addon should sit in `openFrameworks/addons/ofxLeapTouch/`.

Dependencies
------------

* [ofxLeapMotion](http://github.com/ofTheo/ofxLeapMotion/)
* ofxGui (part of the oF core)
* optional: [ofxTuioWrapper](http://github.com/fx-lange/ofxTuioWrapper/) (example-leapToTuio)
* optional: [ofxTangibleUI](http://github.com/fx-lange/ofxTangibleUI/) (example-touchlessUI)
