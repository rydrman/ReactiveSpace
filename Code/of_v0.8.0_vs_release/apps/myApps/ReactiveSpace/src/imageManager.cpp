#include "ImageManager.h"


ImageManager::ImageManager()
{
	if (ofGetWindowWidth() != 1920 || ofGetWindowHeight() != 1080)
	{
		float scaleX = ofGetWindowWidth() / 1920.f;
		float scaleY = ofGetWindowHeight() / 1080.f;
		scaleFactor = (scaleX > scaleY) ? scaleX : scaleY;
	}
	else
	{
		scaleFactor = 1.0;
	}
}


ofImage* ImageManager::load(string filename, bool scale)
{
	for (vector<imageObj>::iterator so = m_loadedImages.begin(); so != m_loadedImages.end(); ++so)
	{
		if (so->filename == filename
			&& so->scaled == scale)
			return so->image;
	}

	imageObj newImageObj;
	newImageObj.image = new ofImage;
	newImageObj.image->loadImage(filename);
	if (scale)
	{
		newImageObj.image->resize(newImageObj.image->getWidth() * scaleFactor,
			newImageObj.image->getHeight() * scaleFactor);
	}
	newImageObj.filename = filename;
	newImageObj.scaled = scale;
	m_loadedImages.push_back(newImageObj);
	return newImageObj.image;
}

float ImageManager::getScaleFactor()
{
	return scaleFactor;
}

ImageManager::~ImageManager()
{
}
