#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "fft.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        //Mode
        int mode;
        int framePhase;
        bool volFlag = true;
        bool autoFlag = true;
        bool switchFlag = false;
    
        //Audio In
        ofSoundStream soundStream;
        void audioIn(float* input, int bufferSize, int nChannels);
    
        float *left, *right;
        float *magnitudeL, *phaseL, *powerL;
        float *magnitudeR, *phaseR, *powerR;
        float avg_powerL, avg_powerR;

        int buffer_size;
        int fft_size;
        
        class fft myfft;

        //GUI
        bool display = false;
        ofxPanel gui;
        ofxSlider<int> angle;
        ofxSlider<int> xCor;
        ofxSlider<int> yCor;
        ofxSlider<int> zCor;
        ofxSlider<float> ratio;
    
};
