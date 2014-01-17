#include "ReactiveSpaceApp.h"

//--------------------------------------------------------------
void ReactiveSpaceApp::setup()
{
	pPeople = new vector<Particle>();

	m_scenes.push_back(new FirstScene(pPeople));

	pCurrentScene = m_scenes[0];
}

//--------------------------------------------------------------
void ReactiveSpaceApp::update(){

}

//--------------------------------------------------------------
void ReactiveSpaceApp::draw(){
	pCurrentScene->Render();
}

//--------------------------------------------------------------
void ReactiveSpaceApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ReactiveSpaceApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ReactiveSpaceApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ReactiveSpaceApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ReactiveSpaceApp::mousePressed(int x, int y, int button)
{
	Particle p;
	p.pos = ofPoint(x, y);
	pPeople->push_back(p);
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
