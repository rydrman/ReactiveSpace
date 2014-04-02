#include "imageManager.h"


imageManager::imageManager()
{
}


ofImage* imageManager::load(string filename)
{
	for (vector<imageObj>::iterator so = m_loadedImages.begin(); so != m_loadedImages.end(); ++so)
	{
		if (so->filename == filename)
			return so->image;
	}

	imageObj newImageObj;
	newImageObj.image = new ofImage;
	newImageObj.image->loadImage(filename);
	newImageObj.filename = filename;
	m_loadedImages.push_back(newImageObj);
	return newImageObj.image;
}



imageManager::~imageManager()
{
}
