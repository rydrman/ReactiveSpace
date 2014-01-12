#include "ReactiveSpaceApp.h"

//--------------------------------------------------------------
void ReactiveSpaceApp::setup(){
	m_path = ofPath();

	m_path.setColor(ofColor(50.f, 80.f, 200.f, 255.f));
	m_path.moveTo(0.f, 0.f);
	m_path.lineTo(10.f, 10.f);
	m_path.lineTo(10.f, 0.f);
}

//--------------------------------------------------------------
void ReactiveSpaceApp::update(){

}

//--------------------------------------------------------------
void ReactiveSpaceApp::draw(){

	for(int i = 0; i < 100; i++)
	{
		m_path.draw( ofRandom(0.f, 1024.f), ofRandom(0.f, 768.f));
	}
}

//--------------------------------------------------------------
void ReactiveSpaceApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ReactiveSpaceApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ReactiveSpaceApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ReactiveSpaceApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ReactiveSpaceApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ReactiveSpaceApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ReactiveSpaceApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ReactiveSpaceApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ReactiveSpaceApp::dragEvent(ofDragInfo dragInfo){ 

}
