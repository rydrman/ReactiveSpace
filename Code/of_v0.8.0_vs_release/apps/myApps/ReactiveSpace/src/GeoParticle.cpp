#include "GeoParticle.h"

GeoParticle::GeoParticle(Particle _old)
:Particle(_old)
{
}

GeoParticle::GeoParticle(ofVec3f _pos)
:Particle(_pos)
{	
	startTime = 0;
	
	//assign a random image
	//int whichGeo = ofRandom(1,6);
	//GeoSize = ofRandom(1,3);
		/*if(whichGeo == 1){
			m_shapes.loadImage("GeoScene/Geometric1.png");
		}
		if(whichGeo == 2){
			m_shapes.loadImage("GeoScene/Geometric2.png");
		}
		if(whichGeo == 3){
			m_shapes.loadImage("GeoScene/Geometric3.png");
		}*/

	//choose random gradient
	float grad = floor(ofRandom(0, 6));
	float gradWidth = 1.f/6.f; 



	for(int x = 0; x < 3; ++x)
	{
		for(int y = 0; y < 3; ++y)
		{
			m_noiseOffset[ 3*x + y ] = ofRandom(10000);
			m_verts[ 3*x + y ] = ofVec2f( -1.f + x + ofNoise( m_noiseOffset[ 3*x + y ] ) * 0.2f, 
										  -1.f + y + ofNoise( m_noiseOffset[ 3*x + y ] ) * 0.2f );
		}
	}

	//texture data
	for(int t = 0; t < 4; ++t)
	{
		for(int i = 0; i < 4; ++i)
		{
			m_texCoords[ (t*4) + i ] = ofVec2f( (gradWidth * grad) + ((i > 0 && i < 3) ? gradWidth : 0), (i < 2) ? 0 : 1);
		}
	}
	m_vbo.setTexCoordData( &m_texCoords[0], 16, GL_STATIC_DRAW);

	update();
}

void GeoParticle::update()
{

	//update vbo positions
	for(int x = 0; x < 3; ++x)
	{
		for(int y = 0; y < 3; ++y)
		{
			m_verts[ 3*x + y ] = ofVec2f( -1 + x - 0.5f + ofNoise( ++m_noiseOffset[ 3*x + y ] * 0.005f ), 
										  -1 + y - 0.5f + ofNoise( (++m_noiseOffset[ 3*x + y ] + 1000) * 0.005f ));
		}
	}
	
	m_vboVerts[0] = m_verts[0];
	m_vboVerts[1] = m_verts[3];
	m_vboVerts[2] = m_verts[4];
	m_vboVerts[3] = m_verts[1];

	m_vboVerts[4] = m_verts[3];
	m_vboVerts[5] = m_verts[6];
	m_vboVerts[6] = m_verts[7];
	m_vboVerts[7] = m_verts[4];

	m_vboVerts[8] = m_verts[1];
	m_vboVerts[9] = m_verts[4];
	m_vboVerts[10] = m_verts[5];
	m_vboVerts[11] = m_verts[2];

	m_vboVerts[12] = m_verts[4];
	m_vboVerts[13] = m_verts[7];
	m_vboVerts[14] = m_verts[8];
	m_vboVerts[15] = m_verts[5];

	m_vbo.setVertexData( &m_vboVerts[0], 16, GL_DYNAMIC_DRAW);
}

void GeoParticle::draw()
{
	ofPushMatrix();
		ofTranslate(pos.x, pos.y);
		ofScale(40.f, 40.f);
		m_vbo.bind();
		glDrawArrays(GL_QUADS, 0, 16);
		m_vbo.unbind();
	ofPopMatrix();
}

void GeoParticle::countDown(int dTime)
{
	//if (vel.x == 0){
			startTime = startTime + dTime;
						
						
			if (startTime > 4000){
				explode();
				startTime = 0;

			}
	//}
}

void GeoParticle::explode()
{
	pos.x = 0;
	
}


GeoParticle::~GeoParticle(void)
{
}
