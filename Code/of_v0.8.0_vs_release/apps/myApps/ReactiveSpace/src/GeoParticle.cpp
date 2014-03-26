#include "GeoParticle.h"

static float s_gradWidth = 1.f / 6.f;
static int s_explosionTimeout = 500;

GeoParticle::GeoParticle(ofVec3f _pos)
: Particle(_pos)
{
	startTime = 0;
	alphaTime = 0;
	m_explodeTime = 0;
	m_isExploding = false;
	m_isDead = false;
	alphaExplode = 250;
	m_vbo.disableVAOs();

	//get a random radius
	GeoSize = ofRandom(ofGetWidth() * 0.05, ofGetWidth() * 0.1);

	//choose random gradient
	m_gradNum = floor(ofRandom(0, 6));

	//get a random number of triangles
	m_numTriangles = (int)ofRandom(3, 7);
	m_triangles = new GeoTriangle[m_numTriangles];

	m_numVerts = m_numTriangles + 1;
	m_originalVerts = new ofVec2f[m_numVerts];
	m_noiseOffsets = new float[m_numVerts];
	m_currentVerts = new ofVec2f[m_numTriangles * 3];;
	m_currentTexCoords = new ofVec2f[m_numTriangles * 3];;

	float angle = PI*2.f / m_numTriangles;

	m_originalVerts[0] = ofVec2f(0.f, 0.f);
	for (int i = 1; i < m_numVerts; ++i)
	{
		m_originalVerts[i] = ofVec2f(cos(angle*i), sin(angle*i));
		m_noiseOffsets[i] = ofRandom(0, 10000);
	}
	
	setTriangles();

	//calculate triangle centers
	for (int i = 0; i < m_numTriangles; ++i)
	{
		m_triangles[i].center = ofVec2f(0.f, 0.f);

		m_triangles[i].center += m_originalVerts[0] + ofNoise(m_noiseOffsets[0]) * 0.2f;
		m_triangles[i].center += m_originalVerts[i + 1] + ofNoise(m_noiseOffsets[i + 1]) * 0.2f;
		if (i == m_numTriangles - 1)
			m_triangles[i].center += m_originalVerts[1] + ofNoise(m_noiseOffsets[1]) * 0.2f;
		else
			m_triangles[i].center += m_originalVerts[i + 2] + ofNoise(m_noiseOffsets[i + 2]) * 0.2f;

		m_triangles[i].center /= 3;
		m_triangles[i].angle = 0;
	}

	//texture data
	for(int i = 0; i < m_numTriangles; ++i)
	{
		getRandomTexCoord(m_triangles[i].texCoords);
	}
	//set to vbo
	for (int i = 0; i < m_numTriangles; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			m_currentTexCoords[i * 3 + j] = m_triangles[i].texCoords[j];
		}
	}
	m_vbo.setTexCoordData(&m_currentTexCoords[0], m_numTriangles * 3, GL_STATIC_DRAW);
}

void GeoParticle::update(float timeScale)
{
	//if exploding, update that sturr
	if (m_isExploding)
	{
		if (m_explodeTime >= s_explosionTimeout)
		{
			m_isDead = true;
			return;
		}
		float explodeSpeed = m_explodeTime / s_explosionTimeout;
		explodeSpeed =  (1.f - explodeSpeed * explodeSpeed * explodeSpeed) * 0.05f;
		for (int i = 0; i < m_numTriangles; ++i)
		{
			float angle = atan2f(m_triangles[i].center.y, m_triangles[i].center.x);
			m_triangles[i].offset += ofVec2f(cos(angle) * explodeSpeed * timeScale, sin(angle) * explodeSpeed * timeScale);
			m_triangles[i].angle += ofNoise(i) * 0.05f;
		}
		m_explodeTime += timeScale * 16.f;
		if (m_explodeTime >= s_explosionTimeout)
			m_explodeTime = s_explosionTimeout;
	}

	// update noise counters
	for (int i = 0; i < m_numVerts; ++i)
	{
		m_noiseOffsets[i] += 0.01f * timeScale;
	}
	setTriangles();

	//build main vbo
	for (int i = 0; i < m_numTriangles; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			m_currentVerts[i * 3 + j] = m_triangles[i].verts[j] + m_triangles[i].offset;
		}
	}
	m_vbo.setVertexData(&m_currentVerts[0], m_numTriangles*3, GL_DYNAMIC_DRAW);


	if(m_isExploding == true){
	alphaTime = alphaTime + timeScale*16.f;
		if (alphaTime > s_explosionTimeout * 0.2){
			alphaExplode = 200;
		}
		if (alphaTime > s_explosionTimeout *0.4){
			alphaExplode = 150;
		}
		if (alphaTime > s_explosionTimeout*0.6){
			alphaExplode = 50;
		}
		if (alphaTime > s_explosionTimeout*0.8){
			alphaExplode = 0;
		}
	}

}

void GeoParticle::draw(ofImage* mGrad, ofImage* explosionSprite)
{
	if(m_isExploding == true && alphaExplode > 0){

		float randRange = 0.3921f * alphaExplode;  // 1/255 * 100
		for(int j = 0 ; j < 10; j ++){
			
			ofPushMatrix();
				ofSetColor(getColor(/*alphaExplode*/));
				explosionSprite->draw(pos.x + ofRandom(-randRange, randRange), pos.y + ofRandom(-randRange, randRange), 30, 30);
			ofPopMatrix();
			
		}
	}
	ofSetColor(255);
	mGrad->bind();

	ofPushMatrix();
		ofTranslate(pos.x, pos.y);
		ofScale(GeoSize, GeoSize);
		//set alpha for explosion
		ofSetColor(255, (int)255 - 255 * (m_explodeTime / s_explosionTimeout));
		m_vbo.bind();
		glDrawArrays(GL_TRIANGLES, 0, m_numTriangles*3);
		m_vbo.unbind();
	ofPopMatrix();

	mGrad->unbind();
}

ofColor GeoParticle::getColor(int alpha)
{
	switch (m_gradNum)
	{
	case 0:
		return ofColor(251, 236, 119, alpha);
	case 1:
		return ofColor(238, 158, 100, alpha);
	case 2:
		return ofColor(211, 75, 63, alpha);
	case 3:
		return ofColor(87, 193, 235, alpha);
	case 4:
		return ofColor(72, 165, 76, alpha);
	case 5:
		return ofColor(169, 233, 180, alpha);
	}

}

void GeoParticle::setTriangles()
{
	for (int i = 0; i < m_numTriangles; ++i)
	{
		m_triangles[i].verts[0] = rotatePoint(m_originalVerts[0], m_triangles[i].center, m_triangles[i].angle) + ofNoise(m_noiseOffsets[0]) * 0.2f;
		m_triangles[i].verts[1] = rotatePoint(m_originalVerts[i + 1], m_triangles[i].center, m_triangles[i].angle) + ofNoise(m_noiseOffsets[i + 1]) * 0.2f;
		if (i == m_numTriangles - 1)
			m_triangles[i].verts[2] = rotatePoint(m_originalVerts[1], m_triangles[i].center, m_triangles[i].angle) + ofNoise(m_noiseOffsets[1]) * 0.2f;
		else
			m_triangles[i].verts[2] = rotatePoint(m_originalVerts[i + 2], m_triangles[i].center, m_triangles[i].angle) + ofNoise(m_noiseOffsets[i + 2]) * 0.2f;
	}
}

ofVec2f GeoParticle::rotatePoint(ofVec2f point, ofVec2f center, float angle)
{
	if (angle == 0)
		return point;
	float curAngle = atan2f(point.y - center.y, point.x - center.x);
	float mag = (point - center).length();

	return ofVec2f(center.x + cos(curAngle + angle) * mag,
				   center.y + sin(curAngle + angle) * mag);
}

void GeoParticle::getRandomTexCoord(ofVec2f* coords)
{
	int startNum = ofRandom(0, 4);
	float gradLeft = m_gradNum * s_gradWidth;

	for (int i = 0; i < 3; ++i)
	{
		switch ((startNum + i) % 4)
		{
		case 0:
			coords[i] = ofVec2f(gradLeft, 0.f);
			break;
		case 1:
			coords[i] = ofVec2f(gradLeft + s_gradWidth, 0.f);
			break;
		case 2:
			coords[i] = ofVec2f(gradLeft + s_gradWidth, 1.f);
			break;
		case 3:
			coords[i] = ofVec2f(gradLeft, 1.f);
			break;
		}
	}
}

void GeoParticle::countDown(float dTime, ofSoundPlayer* geoExplosionSound, ofSoundPlayer* geoExplosionSound2)
{
	//if (vel.x == 0){
			startTime = startTime + dTime*16.f;
						
						
			if (startTime > 2000){
				int coin = ofRandom(2);
				if(coin ==0){geoExplosionSound->play();}
				if(coin==1){geoExplosionSound2->play();}
				
				explode();
				startTime = 0;
			}
	//}
}

void GeoParticle::explode()
{
	if (!m_isExploding)
	{
		m_isExploding = true;
		m_explodeTime = 0;


	}
}


GeoParticle::~GeoParticle(void)
{
}
