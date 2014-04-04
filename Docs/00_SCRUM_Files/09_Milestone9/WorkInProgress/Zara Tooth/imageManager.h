#pragma once
#include "ofMain.h"

using std::vector;

struct imageObj
{
	ofImage* image;
	string filename;
};

class imageManager
{
	vector<imageObj> m_loadedImages;
public:
	imageManager();

	ofImage* load(string filename);

	~imageManager();
};

