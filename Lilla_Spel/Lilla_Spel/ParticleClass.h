#ifndef PARTICLECLASS_H
#define PARTICLECLASS_H

#include "Includes.h"
#include "Particle.h"

//http://www.rastertek.com/dx11tut39.html //referens

class ParticleClass
{
private:
	Particle* particles;	//array med partikalrna 

	//devices
	ID3D11Device* gDevice;
	ID3D11DeviceContext* gDeviceContext;

	//partikel spawner satta saker
	float centerPoint;
	float radius;
	float spawningInterval;

	//partikel spawner randomizer grejer
	float pitch; //runt Y
	float yaw;	 //runt X
	float breadthOnAngle;

	//counters
	int amountOfParticlers;
	float accumulatedTime;

	//info om verticerna
	int mVertexCount;
	int mIndexCount;
	VertexType* mVertices;

	//buffers
	ID3D11Buffer* mVertexBuffer;

	//random grejer som gör så partiklar kan spawna härifrån
	ParticleType* m_particleList;

	float m_particleDeviationX, m_particleDeviationY, m_particleDeviationZ;
	float m_particleVelocity, m_particleVelocityVariation;
	float m_particleSize, m_particlesPerSecond;
	int m_maxParticles;

public:
	ParticleClass(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext);
	ParticleClass(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext, const ParticleClass& particleClass);
	~ParticleClass();

	bool initialize(WCHAR* textureFileName);
	void kill();
	bool frameUpdate(float frameTime); // ska vi ha frame time?
	void render();
	void initiateParticles();
	void shutdownParticleSystem();
	void initializeBuffers();
	void shutdownBuffers();
	void emitParticles(float frameTime);
	void updateParticles(float frameTime);
	void killParticles();
	void updateBuffers();
	void renderBuffers();

	int getmIndexCount();

};

#endif 
