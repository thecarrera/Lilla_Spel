#ifndef PARTICLECLASS_H
#define PARTICLECLASS_H

#include "Includes.h"
#include "Particle.h"

//http://www.rastertek.com/dx11tut39.html //referens

class ParticleClass
{
private:
	Particle* particles;	//array med partikalrna 

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
	VertexType* mVertices; //array med vertiser

	//buffers
	ID3D11Buffer* mVertexBuffer;

	//random grejer som gör så partiklar kan spawna härifrån
	ParticleType* m_particleList; //array med partiklar

	float m_particleDeviationX, m_particleDeviationY, m_particleDeviationZ;
	float m_particleVelocity, m_particleVelocityVariation;
	float m_particleSize, m_particlesPerSecond;
	int m_maxParticles;

public:
	ParticleClass();
	ParticleClass(const ParticleClass& particleClass);
	~ParticleClass();

	void initialize(ID3D11Device* gDevice);
	void kill();
	void frameUpdate(float frameTime, ID3D11DeviceContext* gDeviceContext); // ska vi ha frame time?
	void render(ID3D11DeviceContext* gDeviceContext);
	void initiateParticles();
	void shutdownParticleSystem();
	void initializeBuffers(ID3D11Device* gDevice);
	void shutdownBuffers();
	void emitParticles(float frameTime);
	void updateParticles(float frameTime);
	void killParticles();
	void updateBuffers(ID3D11DeviceContext* gDeviceContext);
	void renderBuffers(ID3D11DeviceContext* gDeviceContext);

	int getmIndexCount();

};

#endif 
