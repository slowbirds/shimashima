#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(0, 0, 0);
    ofEnableAlphaBlending();
    
    // 0 output channels,
    // 2 input channels
    // 44100 samples per second
    // 256 samples per buffer
    // 4 num buffers (latency)
    //soundStream.listDevices();
    //ofSoundStreamListDevices();
    //soundStream.setDeviceID(0);
    
    //MODE
    mode = 1;

    //fft_size = 4096;
    fft_size = 1024;
    buffer_size = fft_size * 2;

    //
    framePhase = 0;
    
    //FFT
    left = new float[buffer_size];
    magnitudeL = new float[fft_size];
    phaseL = new float[fft_size];
    powerL = new float[fft_size];
    
    right = new float[buffer_size];
    magnitudeR = new float[fft_size];
    phaseR = new float[fft_size];
    powerR = new float[fft_size];

    //soundStream.setup(this, 0, 2, 44100, buffer_size, 4);
    ofSoundStreamSetup(0, 2, this, 44100, buffer_size, 4);
    
    //GUI
    gui.setup();
    gui.add(angle.setup("angle", 180, 0, 360));
    gui.add(xCor.setup("xCor", 0, -1, 1));
    gui.add(yCor.setup("yCor", 0, -1, 1));
    gui.add(zCor.setup("zCor", 1, -1, 1));
    gui.add(ratio.setup("ratio", 0.7, 0.2, 1.0));
    
    //ofToggleFullscreen();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //Audio In
    avg_powerL = 0.0;
    avg_powerR = 0.0;
    myfft.powerSpectrum(0, fft_size, left, buffer_size,	magnitudeL, phaseL, powerL, &avg_powerL);
    myfft.powerSpectrum(0, fft_size, right, buffer_size, magnitudeR, phaseR, powerR, &avg_powerR);
}

//--------------------------------------------------------------
void ofApp::draw(){
    //printf("avg_power: %f %f \n", avg_powerL, avg_powerR);
    
    float volL = ofMap(avg_powerL, 0.0, 9.0, 0.0, 255.0);
    float volR = ofMap(avg_powerL, 0.0, 9.0, 0.0, 255.0);
    if (volFlag == false) {
        volL = 255.0;
        volR = 255.0;
    }

    if (autoFlag == true) {
        int a = floor(fmod(ofGetElapsedTimef(), 5));
        if(a == 0){
            if(switchFlag == false){
                switchFlag = true;
                mode = ofRandom(8) + 1;
            }
        }else{
            switchFlag = false;
        }
        //printf("mode: %i %i \n", mode, a);
    }
    

    
    
    if( mode == 1 ){//Phase
        float h = (float)ofGetHeight()/ (float)fft_size;

        ofFill();
        ofColor col;
        
        for (int i = 0; i < fft_size; i++){
            if (phaseL[i] > 0.0) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            
            ofSetColor(col, volL);
            ofRect(0, h * i, ofGetWidth()/2, h);
        }
        for (int i = 0; i < fft_size; i++){
            if (phaseR[i] > 0.0) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volR);
            ofRect(ofGetWidth()/2, h * i, ofGetWidth()/2, h);
        }
    }else if( mode == 2){//Volume
        float h = (float)ofGetHeight()/ (float)fft_size * 1.4;
        
        ofFill();
        ofColor col;
        
        for (int i = 0; i < fft_size; i++){
            if (magnitudeL[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volL);
            ofRect(0, h * i, ofGetWidth()/2, h);
        }
        for (int i = 0; i < fft_size; i++){
            if (magnitudeR[i] > 0.5) {
                //printf("magnitudeL: %f \n", magnitudeL[i]);
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volR);
            ofRect(ofGetWidth()/2, h * i, ofGetWidth()/2, h);
        }
        
    }else if( mode == 3){//Volume2 Random
        float h = (float)ofGetHeight()/ (float)fft_size * 1.4;
        ofFill();
        ofColor col;
        
        ofPushMatrix();
        for (int i = 0; i < fft_size; i++){
            int random = ofRandom(fft_size);
            
            if (magnitudeL[random] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volL);
            ofRect(0, h * i, ofGetWidth()/2, h);
        }
        
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        ofRotate(angle, xCor, yCor, zCor);
        
        for (int i = 0; i < fft_size; i++){
            int random = ofRandom(fft_size);
            
            if (magnitudeR[random] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volR);
            ofRect(-ofGetWidth()/2, h * i - ofGetHeight()/2, ofGetWidth()/2, h);
        }
        
        ofPopMatrix();
    }else if( mode == 4){//LINE 2
        int tempFft = fft_size * ratio;
        
        float h = (float)ofGetHeight()/ (float)tempFft;
        ofPushMatrix();
        
        ofFill();
        ofColor col;
        
        for (int i = 0; i < tempFft; i++){
            if (magnitudeL[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volL);
            ofRect(0, h * i, ofGetWidth()/2, h);
        }
        
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        ofRotate(angle, xCor, yCor, zCor);
        
        for (int i = 0; i < tempFft; i++){
            if (magnitudeR[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volR);
            ofRect(-ofGetWidth()/2, h * i - ofGetHeight()/2, ofGetWidth()/2, h);
        }
        
        ofPopMatrix();
    }else if( mode == 5){//LINE 2 MOVE
        int tempFft = fft_size * ratio;
        float h = (float)ofGetHeight()/ (float)tempFft;
        
        //int a = floor(fmod(ofGetElapsedTimef(), fft_size * 2));
        framePhase = framePhase + 10;
        if(framePhase > ofGetHeight()) framePhase = 0;

        ofFill();
        ofColor col;
        
        ofPushMatrix();
        ofTranslate(0, framePhase);
        
        for (int i = 0; i < tempFft; i++){
            if (magnitudeL[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volL);
            ofRect(0, h * i - ofGetHeight(), ofGetWidth()/2, h);
            ofRect(0, h * i, ofGetWidth()/2, h);
            ofRect(0, h * i + ofGetHeight(), ofGetWidth()/2, h);
        }

        ofPopMatrix();
        ofPushMatrix();
        
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2 - framePhase);
        ofRotate(angle, xCor, yCor, zCor);
        
        for (int i = 0; i < tempFft; i++){
            if (magnitudeR[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volR);
            ofRect(-ofGetWidth()/2, h * i - ofGetHeight()/2 - ofGetHeight(), ofGetWidth()/2, h);
            ofRect(-ofGetWidth()/2, h * i - ofGetHeight()/2, ofGetWidth()/2, h);
            ofRect(-ofGetWidth()/2, h * i - ofGetHeight()/2 + ofGetHeight(), ofGetWidth()/2, h);
        }
        
        ofPopMatrix();
    }else if( mode == 6){//LINE 4
        int tempFft = fft_size * ratio;
        float h = (float)ofGetHeight()/ (float)tempFft;
        
        ofFill();
        ofColor col;
        
        ofPushMatrix();
        
        for (int i = 0; i < tempFft; i++){
            if (magnitudeL[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volL);
            ofRect(0, h * i - ofGetHeight(), ofGetWidth()/4, h);
            ofRect(0, h * i, ofGetWidth()/4, h);
            ofRect(0, h * i + ofGetHeight(), ofGetWidth()/4, h);
        }
        
        ofPopMatrix();
        ofPushMatrix();
        
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        ofRotate(angle, xCor, yCor, zCor);
        
        for (int i = 0; i < tempFft; i++){
            if (magnitudeR[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volR);
            ofRect(0, h * i - ofGetHeight()/2 - ofGetHeight(), ofGetWidth()/4, h);
            ofRect(0, h * i - ofGetHeight()/2, ofGetWidth()/4, h);
            ofRect(0, h * i - ofGetHeight()/2 + ofGetHeight(), ofGetWidth()/4, h);
        }
        
        ofPopMatrix();
        ofPushMatrix();
        
        for (int i = 0; i < tempFft; i++){
            if (magnitudeL[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volL);
            ofRect(ofGetWidth()/2, h * i - ofGetHeight(), ofGetWidth()/4, h);
            ofRect(ofGetWidth()/2, h * i, ofGetWidth()/4, h);
            ofRect(ofGetWidth()/2, h * i + ofGetHeight(), ofGetWidth()/4, h);
        }
        
        ofPopMatrix();
        ofPushMatrix();
        
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        ofRotate(angle, xCor, yCor, zCor);
        
        for (int i = 0; i < tempFft; i++){
            if (magnitudeR[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volR);
            ofRect(-ofGetWidth()/2, h * i - ofGetHeight()/2 - ofGetHeight(), ofGetWidth()/4, h);
            ofRect(-ofGetWidth()/2, h * i - ofGetHeight()/2, ofGetWidth()/4, h);
            ofRect(-ofGetWidth()/2, h * i - ofGetHeight()/2 + ofGetHeight(), ofGetWidth()/4, h);
        }
        
        ofPopMatrix();
    }else if( mode == 7){//LINE 4 MOVE
        int tempFft = fft_size * ratio;
        float h = (float)ofGetHeight()/ (float)tempFft;
        
        //int a = floor(fmod(ofGetElapsedTimef(), fft_size * 2));
        framePhase = framePhase + 10;
        if(framePhase > ofGetHeight()) framePhase = 0;
        
        ofFill();
        ofColor col;
        
        ofPushMatrix();
        ofTranslate(0, framePhase);
        
        for (int i = 0; i < tempFft; i++){
            if (magnitudeL[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volL);
            ofRect(0, h * i - ofGetHeight(), ofGetWidth()/4, h);
            ofRect(0, h * i, ofGetWidth()/4, h);
            ofRect(0, h * i + ofGetHeight(), ofGetWidth()/4, h);
        }
        
        ofPopMatrix();
        ofPushMatrix();
        
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2 - framePhase);
        ofRotate(angle, xCor, yCor, zCor);
        
        for (int i = 0; i < tempFft; i++){
            if (magnitudeR[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volR);
            ofRect(0, h * i - ofGetHeight()/2 - ofGetHeight(), ofGetWidth()/4, h);
            ofRect(0, h * i - ofGetHeight()/2, ofGetWidth()/4, h);
            ofRect(0, h * i - ofGetHeight()/2 + ofGetHeight(), ofGetWidth()/4, h);
        }
        
        ofPopMatrix();
        ofPushMatrix();
        
        ofTranslate(0, framePhase);
        
        for (int i = 0; i < tempFft; i++){
            if (magnitudeL[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volL);
            ofRect(ofGetWidth()/2, h * i - ofGetHeight(), ofGetWidth()/4, h);
            ofRect(ofGetWidth()/2, h * i, ofGetWidth()/4, h);
            ofRect(ofGetWidth()/2, h * i + ofGetHeight(), ofGetWidth()/4, h);
        }
        
        ofPopMatrix();
        ofPushMatrix();
        
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2 - framePhase);
        ofRotate(angle, xCor, yCor, zCor);
        
        for (int i = 0; i < tempFft; i++){
            if (magnitudeR[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volR);
            ofRect(-ofGetWidth()/2, h * i - ofGetHeight()/2 - ofGetHeight(), ofGetWidth()/4, h);
            ofRect(-ofGetWidth()/2, h * i - ofGetHeight()/2, ofGetWidth()/4, h);
            ofRect(-ofGetWidth()/2, h * i - ofGetHeight()/2 + ofGetHeight(), ofGetWidth()/4, h);
        }
        
        ofPopMatrix();
    }else if( mode == 8){//LINE 8
        int lineNum = 8;
        int tempFft = fft_size * ratio;
        float h = (float)ofGetHeight()/ (float)tempFft;
        
        ofFill();
        ofColor col;
        
        ofPushMatrix();//Line 1
        for (int i = 0; i < tempFft; i++){
            if (magnitudeL[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volL);
            ofRect(0, h * i - ofGetHeight(), ofGetWidth()/lineNum, h);
            ofRect(0, h * i, ofGetWidth()/lineNum, h);
            ofRect(0, h * i + ofGetHeight(), ofGetWidth()/lineNum, h);
        }
        ofPopMatrix();
        ofPushMatrix();//Line 2
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        ofRotate(angle, xCor, yCor, zCor);
        for (int i = 0; i < tempFft; i++){
            if (magnitudeR[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volR);
            ofRect(ofGetWidth()/4, h * i - ofGetHeight()/2 - ofGetHeight(), ofGetWidth()/lineNum, h);
            ofRect(ofGetWidth()/4, h * i - ofGetHeight()/2, ofGetWidth()/lineNum, h);
            ofRect(ofGetWidth()/4, h * i - ofGetHeight()/2 + ofGetHeight(), ofGetWidth()/lineNum, h);
        }
        ofPopMatrix();
        ofPushMatrix();//Line 3
        for (int i = 0; i < tempFft; i++){
            if (magnitudeL[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volL);
            ofRect(ofGetWidth()/4, h * i - ofGetHeight(), ofGetWidth()/lineNum, h);
            ofRect(ofGetWidth()/4, h * i, ofGetWidth()/lineNum, h);
            ofRect(ofGetWidth()/4, h * i + ofGetHeight(), ofGetWidth()/lineNum, h);
        }
        ofPopMatrix();
        ofPushMatrix();//Line 4
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        ofRotate(angle, xCor, yCor, zCor);
        
        for (int i = 0; i < tempFft; i++){
            if (magnitudeR[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volR);
            ofRect(0, h * i - ofGetHeight()/2 - ofGetHeight(), ofGetWidth()/lineNum, h);
            ofRect(0, h * i - ofGetHeight()/2, ofGetWidth()/lineNum, h);
            ofRect(0, h * i - ofGetHeight()/2 + ofGetHeight(), ofGetWidth()/lineNum, h);
        }
        ofPopMatrix();
        ofPushMatrix();//Line 5
        for (int i = 0; i < tempFft; i++){
            if (magnitudeL[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volL);
            ofRect(ofGetWidth()/2, h * i - ofGetHeight(), ofGetWidth()/lineNum, h);
            ofRect(ofGetWidth()/2, h * i, ofGetWidth()/lineNum, h);
            ofRect(ofGetWidth()/2, h * i + ofGetHeight(), ofGetWidth()/lineNum, h);
        }
        ofPopMatrix();
        ofPushMatrix();//Line 6
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        ofRotate(angle, xCor, yCor, zCor);
        for (int i = 0; i < tempFft; i++){
            if (magnitudeR[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volR);
            ofRect(-ofGetWidth()/4, h * i - ofGetHeight()/2 - ofGetHeight(), ofGetWidth()/lineNum, h);
            ofRect(-ofGetWidth()/4, h * i - ofGetHeight()/2, ofGetWidth()/lineNum, h);
            ofRect(-ofGetWidth()/4, h * i - ofGetHeight()/2 + ofGetHeight(), ofGetWidth()/lineNum, h);
        }
        ofPopMatrix();
        ofPushMatrix();//Line 7
        for (int i = 0; i < tempFft; i++){
            if (magnitudeL[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volL);
            ofRect(ofGetWidth()/2 + ofGetWidth()/4, h * i - ofGetHeight(), ofGetWidth()/lineNum, h);
            ofRect(ofGetWidth()/2 + ofGetWidth()/4, h * i, ofGetWidth()/lineNum, h);
            ofRect(ofGetWidth()/2 + ofGetWidth()/4, h * i + ofGetHeight(), ofGetWidth()/lineNum, h);
        }
        ofPopMatrix();
        ofPushMatrix();//Line 8
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        ofRotate(angle, xCor, yCor, zCor);
        
        for (int i = 0; i < tempFft; i++){
            if (magnitudeR[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volR);
            ofRect(-ofGetWidth()/2, h * i - ofGetHeight()/2 - ofGetHeight(), ofGetWidth()/lineNum, h);
            ofRect(-ofGetWidth()/2, h * i - ofGetHeight()/2, ofGetWidth()/lineNum, h);
            ofRect(-ofGetWidth()/2, h * i - ofGetHeight()/2 + ofGetHeight(), ofGetWidth()/lineNum, h);
        }
        ofPopMatrix();
    }else if( mode == 9){//LINE8 MOVE
        int lineNum = 8;
        int tempFft = fft_size * ratio;
        float h = (float)ofGetHeight()/ (float)tempFft;
        
        //int a = floor(fmod(ofGetElapsedTimef(), fft_size * 2));
        framePhase = framePhase + 10;
        if(framePhase > ofGetHeight()) framePhase = 0;
        
        ofFill();
        ofColor col;
        
        ofPushMatrix();//Line 1
        ofTranslate(0, framePhase);
        for (int i = 0; i < tempFft; i++){
            if (magnitudeL[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volL);
            ofRect(0, h * i - ofGetHeight(), ofGetWidth()/lineNum, h);
            ofRect(0, h * i, ofGetWidth()/lineNum, h);
            ofRect(0, h * i + ofGetHeight(), ofGetWidth()/lineNum, h);
        }
        ofPopMatrix();
        ofPushMatrix();//Line 2
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2 - framePhase);
        ofRotate(angle, xCor, yCor, zCor);
        for (int i = 0; i < tempFft; i++){
            if (magnitudeR[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volR);
            ofRect(ofGetWidth()/4, h * i - ofGetHeight()/2 - ofGetHeight(), ofGetWidth()/lineNum, h);
            ofRect(ofGetWidth()/4, h * i - ofGetHeight()/2, ofGetWidth()/lineNum, h);
            ofRect(ofGetWidth()/4, h * i - ofGetHeight()/2 + ofGetHeight(), ofGetWidth()/lineNum, h);
        }
        ofPopMatrix();
        ofPushMatrix();//Line 3
        ofTranslate(0, framePhase);
        for (int i = 0; i < tempFft; i++){
            if (magnitudeL[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volL);
            ofRect(ofGetWidth()/4, h * i - ofGetHeight(), ofGetWidth()/lineNum, h);
            ofRect(ofGetWidth()/4, h * i, ofGetWidth()/lineNum, h);
            ofRect(ofGetWidth()/4, h * i + ofGetHeight(), ofGetWidth()/lineNum, h);
        }
        ofPopMatrix();
        ofPushMatrix();//Line 4
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2 - framePhase);
        ofRotate(angle, xCor, yCor, zCor);
        
        for (int i = 0; i < tempFft; i++){
            if (magnitudeR[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volR);
            ofRect(0, h * i - ofGetHeight()/2 - ofGetHeight(), ofGetWidth()/lineNum, h);
            ofRect(0, h * i - ofGetHeight()/2, ofGetWidth()/lineNum, h);
            ofRect(0, h * i - ofGetHeight()/2 + ofGetHeight(), ofGetWidth()/lineNum, h);
        }
        ofPopMatrix();
        ofPushMatrix();//Line 5
        ofTranslate(0, framePhase);
        for (int i = 0; i < tempFft; i++){
            if (magnitudeL[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volL);
            ofRect(ofGetWidth()/2, h * i - ofGetHeight(), ofGetWidth()/lineNum, h);
            ofRect(ofGetWidth()/2, h * i, ofGetWidth()/lineNum, h);
            ofRect(ofGetWidth()/2, h * i + ofGetHeight(), ofGetWidth()/lineNum, h);
        }
        ofPopMatrix();
        ofPushMatrix();//Line 6
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2 - framePhase);
        ofRotate(angle, xCor, yCor, zCor);
        for (int i = 0; i < tempFft; i++){
            if (magnitudeR[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volR);
            ofRect(-ofGetWidth()/4, h * i - ofGetHeight()/2 - ofGetHeight(), ofGetWidth()/lineNum, h);
            ofRect(-ofGetWidth()/4, h * i - ofGetHeight()/2, ofGetWidth()/lineNum, h);
            ofRect(-ofGetWidth()/4, h * i - ofGetHeight()/2 + ofGetHeight(), ofGetWidth()/lineNum, h);
        }
        ofPopMatrix();
        ofPushMatrix();//Line 7
        ofTranslate(0, framePhase);
        for (int i = 0; i < tempFft; i++){
            if (magnitudeL[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volL);
            ofRect(ofGetWidth()/2 + ofGetWidth()/4, h * i - ofGetHeight(), ofGetWidth()/lineNum, h);
            ofRect(ofGetWidth()/2 + ofGetWidth()/4, h * i, ofGetWidth()/lineNum, h);
            ofRect(ofGetWidth()/2 + ofGetWidth()/4, h * i + ofGetHeight(), ofGetWidth()/lineNum, h);
        }
        ofPopMatrix();
        ofPushMatrix();//Line 8
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2 - framePhase);
        ofRotate(angle, xCor, yCor, zCor);
        
        for (int i = 0; i < tempFft; i++){
            if (magnitudeR[i] > 0.5) {
                col.set(255.0, 255.0, 255.0);
            }else{
                col.set(0.0, 0.0, 0.0);
            }
            ofSetColor(col, volR);
            ofRect(-ofGetWidth()/2, h * i - ofGetHeight()/2 - ofGetHeight(), ofGetWidth()/lineNum, h);
            ofRect(-ofGetWidth()/2, h * i - ofGetHeight()/2, ofGetWidth()/lineNum, h);
            ofRect(-ofGetWidth()/2, h * i - ofGetHeight()/2 + ofGetHeight(), ofGetWidth()/lineNum, h);
        }
        ofPopMatrix();
    }

    if(display){
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    //printf("bufferSize: %i \n", bufferSize);
    //bufferSize = 4096
    
    for (int i = 0; i < bufferSize; i++){
        //printf("left: %f, %f \n", input[i],input[i*2]);
        
        left[i]		= input[i*2];
        right[i]	= input[i*2+1];
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == '1' ){
        mode = 1;
    }else if( key == '2'){
        mode = 2;
    }else if( key == '3'){
        mode = 3;
    }else if( key == '4'){
        mode = 4;
    }else if( key == '5'){
        mode = 5;
    }else if( key == '6'){
        mode = 6;
    }else if( key == '7'){
        mode = 7;
    }else if( key == '8'){
        mode = 8;
    }else if( key == '9'){
        mode = 9;
    }
    
    //Full Screen
    if(key == 'f'){
        ofToggleFullscreen();
    }

    //GUI
    if(key == 'd'){
        display = !display;
    }

    //Vol
    if(key == 'v'){
        volFlag = !volFlag;
    }

    //Auto Play
    if(key == 'a'){
        autoFlag = !autoFlag;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
