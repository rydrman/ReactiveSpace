#include "GeoParticle.h"

static float s_gradWidth = 1.f / 6.f;

GeoParticle::GeoParticle(Particle _old)
:Particle(_old)
{
}

GeoParticle::GeoParticle(ofVec3f _pos)
: Particle(_pos)
{
	startTime = 0;
	alphaTime = 0;
	m_explodeTime = 0;
	m_isExploding = false;
	alphaExplode = 250;

	

	//geoExplodeSound.loadSound("GeoScene/Scene3_Explosion.mp3");
	 
	


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
			cout << "nums: " << i << " " << j << " " << (i * 3 + j) << " : " << m_triangles[i].texCoords[j] << endl;
		}
	}
	m_vbo.setTexCoordData(&m_currentTexCoords[0], m_numTriangles * 3, GL_STATIC_DRAW);
}

void GeoParticle::update(float timeScale)
{
	//if exploding, update that sturr
	if (m_isExploding)
	{
		for (int i = 0; i < m_numTriangles; ++i)
		{
			float angle = atan2f(m_triangles[i].center.y, m_triangles[i].center.x);
			m_triangles[i].offset += ofVec2f(cos(angle) * 0.5f * timeScale, sin(angle) * 0.5f * timeScale);
		}
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
		if (alphaTime > 200){
			alphaExplode = 200;
		}
		if (alphaTime > 400){
			alphaExplode = 150;
		}
		if (alphaTime > 600){
			alphaExplode = 50;
		}
		if (alphaTime > 800){
			alphaExplode = 0;
		}
	}

}

void GeoParticle::draw(ofImage mGrad, ofImage explosionSprite)
{
	if(m_isExploding == true){
		
		for(int j = 0 ; j < 50; j ++){
			
			ofPushMatrix();

				if(m_gradNum == 0 ){ofSetColor(251,236,119,alphaExplode);}
				if(m_gradNum == 1 ){ofSetColor(238,158,100,alphaExplode);}
				if(m_gradNum == 2 ){ofSetColor(211,75,63,alphaExplode);}
				if(m_gradNum == 3 ){ofSetColor(87,193,235,alphaExplode);}
				if(m_gradNum == 4 ){ofSetColor(72,165,76,alphaExplode);}
				if(m_gradNum == 5 ){ofSetColor(169,233,180,alphaExplode);}

				explosionSprite.draw(pos.x+ofRandom(-alphaExplode,alphaExplode),pos.y+ofRandom(-alphaExplode,alphaExplode), 30,30);
			ofPopMatrix();
			
		}
	}
	ofSetColor(255);
	mGrad.bind();

	ofPushMatrix();
		ofTranslate(pos.x, pos.y);
		ofScale(GeoSize, GeoSize);
		m_vbo.bind();
		glDrawArrays(GL_TRIANGLES, 0, m_numTriangles*3);
		m_vbo.unbind();
	ofPopMatrix();

	mGrad.unbind();

	
}

void GeoParticle::setTriangles()
{
	for (int i = 0; i < m_numTriangles; ++i)
	{
		m_triangles[i].verts[0] = m_originalVerts[0] + ofNoise(m_noiseOffsets[0]) * 0.2f;
		m_triangles[i].verts[1] = m_originalVerts[i + 1] + ofNoise(m_noiseOffsets[i + 1]) * 0.2f;
		if (i == m_numTriangles - 1)
			m_triangles[i].verts[2] = m_originalVerts[1] + ofNoise(m_noiseOffsets[1]) * 0.2f;
		else
			m_triangles[i].verts[2] = m_originalVerts[i + 2] + ofNoise(m_noiseOffsets[i + 2]) * 0.2f;
	}
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

void GeoParticle::countDown(float dTime)
{
	//if (vel.x == 0){
			startTime = startTime + dTime*16.f;
			//geoExplodeSound.play();			
						
			if (startTime > 2000){
				explode();
				startTime = 0;
			}
	//}
}

void GeoParticle::explode()
{
	m_isExploding = true;
	m_explodeTime = 0;
	vel.x = 0;
}


GeoParticle::~GeoParticle(void)
{
}
