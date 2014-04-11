#pragma once
#include "ofMain.h"

using std::vector;

struct imageObj
{
	ofImage* image;
	string filename;
	bool scaled;
};

class ImageManager
{
	vector<imageObj> m_loadedImages;
	float scaleFactor;

public:
	ImageManager();

	ofImage* load(string filename, bool scale = true);
	float getScaleFactor();

	~ImageManager();
};

