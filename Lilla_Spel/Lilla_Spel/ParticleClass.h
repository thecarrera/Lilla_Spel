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

	//texture
	ID3D11Resource* textureResoure;
	ID3D11ShaderResourceView* textureView;
	
	//info om verticerna
	int mVertexCount;
	int mIndexCount;
	vertexType* mVertices;

	//buffers
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;

public:
	ParticleClass(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext);
	ParticleClass(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext, const ParticleClass& particleClass);
	~ParticleClass();

	bool initialize(WCHAR* textureFileName);
	void kill();
	bool frameUpdate(float frameTime); // ska vi ha frame time?
	void render();
	//function för return texture
	void createTexture();
	void initiateParticles();

	void setVertexBuffer();



	int getmIndexCount();

};

#endif 
