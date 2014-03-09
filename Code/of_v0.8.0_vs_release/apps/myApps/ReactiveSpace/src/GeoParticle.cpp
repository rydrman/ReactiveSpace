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
			
	//create random vertices and push to GPU
	for(int i = 0; i < 4; ++i)
	{
		m_verts[i] = ofVec2f( (i > 0 && i < 3) ? 1 : -1, (i < 2) ? -1 : 1);
		m_texCoords[i] = ofVec2f( (i > 0 && i < 3) ? 1 : 0, (i < 2) ? 0 : 1);
		m_texCoords[i] = ofVec2f( (gradWidth * grad) + ((i > 0 && i < 3) ? gradWidth : 0), (i < 2) ? 0 : 1);
	}
	m_vbo.setVertexData( &m_verts[0], 4, GL_DYNAMIC_DRAW);
	m_vbo.setTexCoordData( &m_texCoords[0], 4, GL_STATIC_DRAW);
}

void GeoParticle::draw()
{
	ofPushMatrix();
		ofTranslate(pos.x, pos.y);
		ofScale(40.f, 40.f);
		m_vbo.bind();
		glDrawArrays(GL_QUADS, 0, 4);
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
