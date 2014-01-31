#include "ofMain.h"
#include <Windows.h>
#include <Ole2.h>
#include <NuiApi.h>
#include "ReactiveSpaceApp.h"

//========================================================================
int main( ){
	ofSetupOpenGL(1920,1080,OF_FULLSCREEN);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ReactiveSpaceApp());

}
